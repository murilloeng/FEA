//std
#include <cmath>
#include <cstring>
#include <stdexcept>

//umfpack
#include <suitesparse/umfpack.h>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Linear/Sparse.hpp"
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Results/What.hpp"
#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Time.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/State.hpp"
#include "FEA/inc/Analysis/Solvers/Drift.hpp"
#include "FEA/inc/Analysis/Solvers/Modal.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Solvers/Buckling.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticLinear.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicLinear.hpp"
#include "FEA/inc/Analysis/Solvers/TransientLinear.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicNonlinear.hpp"
#include "FEA/inc/Analysis/Solvers/TransientNonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Solver::Solver(void) :
				m_stop(false), m_watch_dof(this), 
				m_stop_criteria(this), m_step_max(100), m_combination(UINT_MAX),
				m_r(nullptr), m_R(nullptr), 
				m_fnu(nullptr), m_fnk(nullptr), 
				m_fiu(nullptr), m_fik(nullptr), 
				m_feu(nullptr), m_fek(nullptr),
				m_fru(nullptr), m_frk(nullptr), 
				m_fdu(nullptr), m_fdk(nullptr),
				m_K(nullptr), m_C(nullptr), m_M(nullptr), m_S(nullptr),
				m_k(nullptr), m_m(nullptr), m_z(nullptr), m_e(nullptr),
				m_dxr(nullptr), m_dxt(nullptr), m_dvt(nullptr),
				m_ddx(nullptr), m_ddxr(nullptr), m_ddxt(nullptr),
				m_x_old(nullptr), m_x_new(nullptr), m_dx(nullptr),
				m_v_old(nullptr), m_v_new(nullptr), m_dv(nullptr),
				m_a_old(nullptr), m_a_new(nullptr), m_da(nullptr),
				m_state_data(nullptr), m_energy_data(nullptr), 
				m_t0(0), m_t_min(0), m_t_max(1), m_dt0(0), m_dt_min(0), m_dt_max(INFINITY),
				m_p0(0), m_p_min(-INFINITY), m_p_max(+INFINITY), m_dp0(1e-2), m_dp_min(-INFINITY), m_dp_max(INFINITY)
			{
				return;
			}

			//destructor
			Solver::~Solver(void)
			{
				cleanup();
			}

			//create
			void Solver::create(Solver*& solver, Type type)
			{
				//data
				delete solver;
				const std::function<void(void)> factory[] = {
					[&solver](void){ solver = new Drift; },
					[&solver](void){ solver = new Modal; },
					[&solver](void){ solver = new State; },
					[&solver](void){ solver = new Buckling; },
					[&solver](void){ solver = new StaticLinear; },
					[&solver](void){ solver = new DynamicLinear; },
					[&solver](void){ solver = new TransientLinear; },
					[&solver](void){ solver = new StaticNonlinear; },
					[&solver](void){ solver = new DynamicNonlinear; },
					[&solver](void){ solver = new TransientNonlinear; }
				};
				//create
				for(uint32_t i = 0; 1U << i < uint32_t(Type::last); i++)
				{
					if(type == Type(1U << i)) factory[i]();
				}
			}

			//serialization
			void Solver::load(FILE* file)
			{
				//data
				const char* format = 
					"%d %d "
					"%lf %lf %lf %lf %lf %lf "
					"%lf %lf %lf %lf %lf %lf ";
				//save
				m_watch_dof.load(file);
				m_stop_criteria.load(file);
				if(fscanf(file, format,
					&m_step_max, &m_combination,
					&m_t0, &m_t_min, &m_t_max, &m_dt0, &m_dt_min, &m_dt_max,
					&m_p0, &m_p_min, &m_p_max, &m_dp0, &m_dp_min, &m_dp_max) != 14)
				{
					throw std::runtime_error("Solver loading error!");
				}
			}
			void Solver::save(FILE* file) const
			{
				//data
				const char* format = 
					"%d %d "
					"%+.6e %+.6e %+.6e %+.6e %+.6e %+.6e "
					"%+.6e %+.6e %+.6e %+.6e %+.6e %+.6e ";
				//save
				m_watch_dof.save(file);
				m_stop_criteria.save(file);
				if(fprintf(file, format,
					m_step_max, m_combination,
					m_t0, m_t_min, m_t_max, m_dt0, m_dt_min, m_dt_max,
					m_p0, m_p_min, m_p_max, m_dp0, m_dp_min, m_dp_max) < 0)
				{
					throw std::runtime_error("Solver saving error!");
				}
			}

			void Solver::load_state(void)
			{
				//data
				char buffer[800];
				const uint32_t ns = m_analysis->model()->results()->steps();
				sprintf(buffer, "%s/Solver/Solver.txt", m_analysis->model()->folder().c_str());
				//check
				if(!m_analysis->model()->results()->what()->solver()) return;
				//allocate
				delete[] m_state_data;
				m_state_data = new double[ns + 1];
				//open
				FILE* file = fopen(buffer, "r");
				//load
				if(!file)
				{
					throw std::runtime_error("Unable to open solver results file!");
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					if(fscanf(file, "%lf", &m_state_data[i]) != 1)
					{
						throw std::runtime_error("Unable to read from solver results file!");
					}
				}
				//close
				fclose(file);
			}
			void Solver::save_state(void) const
			{
				//data
				char buffer[800];
				const uint32_t ns = m_analysis->model()->results()->steps();
				sprintf(buffer, "%s/Solver/Solver.txt", m_analysis->model()->folder().c_str());
				//check
				if(!m_analysis->model()->results()->what()->solver()) return;
				//open
				FILE* file = fopen(buffer, "w");
				//save
				if(!file)
				{
					throw std::runtime_error("Unable to create solver results file!");
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					if(fprintf(file, "%+.6e\n", m_state_data[i]) < 0)
					{
						fclose(file);
						throw std::runtime_error("Unable to write to solver results file!");
					}
				}
				//close
				fclose(file);
			}

			void Solver::load_energy(void)
			{
				//data
				char path[800];
				const uint32_t ns = m_analysis->model()->results()->steps();
				sprintf(path, "%s/Energy/Total.txt", m_analysis->model()->folder().c_str());
				//check
				if(!m_analysis->model()->results()->what()->energies()) return;
				//allocate
				delete[] m_energy_data;
				m_energy_data = new double[2 * (ns + 1)];
				//open
				FILE* file = fopen(path, "r");
				//load
				if(!file)
				{
					throw std::runtime_error("Unable to open solver energy results file!");
				}
				for(uint32_t i = 0; i < 2 * (ns + 1); i++)
				{
					if(fscanf(file, "%lf", &m_energy_data[i]) != 1)
					{
						throw std::runtime_error("Unable to read from solver energy results file!");
					}
				}
				//close
				fclose(file);
			}
			void Solver::save_energy(void) const
			{
				//data
				char path[800];
				const uint32_t ns = m_analysis->model()->results()->steps();
				sprintf(path, "%s/Energy/Total.txt", m_analysis->model()->folder().c_str());
				//check
				if(!m_analysis->model()->results()->what()->energies()) return;
				//open
				FILE* file = fopen(path, "w");
				//save
				if(!file)
				{
					throw std::runtime_error("Unable to create solver energy results file!");
				}
				for(uint32_t i = 0; i < 2 * (ns + 1); i++)
				{
					if(fprintf(file, "%+.6e ", m_energy_data[i]) < 0)
					{
						fclose(file);
						throw std::runtime_error("Unable to write to solver energy results file!");
					}
					if(i % 2 == 1) fprintf(file, "\n");
				}
				//close
				fclose(file);
			}

			//data
			void Solver::mark(void)
			{
				m_stop = true;
			}

			uint32_t Solver::step(void) const
			{
				return m_step;
			}
			uint32_t Solver::step_max(void) const
			{
				return m_step_max;
			}
			uint32_t Solver::step_max(uint32_t step_max)
			{
				return m_step_max = step_max;
			}

			Analysis* Solver::analysis(void)
			{
				return m_analysis;
			}

			StopCriteria& Solver::stop_criteria(void)
			{
				return m_stop_criteria;
			}
			StopCriteria& Solver::stop_criteria(uint32_t types)
			{
				m_stop_criteria.m_types = types;
				return m_stop_criteria;
			}
			const StopCriteria& Solver::stop_criteria(void) const
			{
				return m_stop_criteria;
			}

			WatchDof& Solver::watch_dof(void)
			{
				return m_watch_dof;
			}
			const WatchDof& Solver::watch_dof(void) const
			{
				return m_watch_dof;
			}
			WatchDof& Solver::watch_dof(uint32_t node, mesh::nodes::dof dof)
			{
				//setup
				m_watch_dof.m_dof = dof;
				m_watch_dof.m_node = node;
				//return
				return m_watch_dof;
			}

			solvers::interface Solver::interface(void) const
			{
				return m_interface;
			}
			solvers::interface Solver::interface(solvers::interface interface)
			{
				return m_interface = interface;
			}

			uint32_t Solver::combination_index(void) const
			{
				return m_combination;
			}
			boundary::loads::LoadCombination* Solver::combination(void) const
			{
				return m_analysis->model()->boundary()->load_combination(m_combination);
			}
			boundary::loads::LoadCombination* Solver::load_combination(uint32_t combination)
			{
				if(combination != UINT_MAX && combination >= m_analysis->model()->boundary()->load_combinations().size())
				{
					throw std::runtime_error("Solver combination is out of range!");
				}
				m_combination = combination;
				return m_analysis->model()->boundary()->load_combination(m_combination);
			}

			//load
			double Solver::load_min(void) const
			{
				return m_p_min;
			}
			double Solver::load_min(double p_min)
			{
				return m_p_min = p_min;
			}

			double Solver::load_max(void) const
			{
				return m_p_max;
			}
			double Solver::load_max(double p_max)
			{
				return m_p_max = p_max;
			}

			double Solver::load_initial(double p0)
			{
				return m_p0 = p0;
			}
			double Solver::load_initial(void) const
			{
				return m_p0;
			}

			double Solver::load_increment_min(void) const
			{
				return m_dp_min;
			}
			double Solver::load_increment_min(double dp_min)
			{
				return m_dp_min;
			}

			double Solver::load_increment_max(void) const
			{
				return m_dp_max;
			}
			double Solver::load_increment_max(double dp_max)
			{
				return m_dp_max;
			}

			double Solver::load_increment_initial(void) const
			{
				return m_dp0;
			}
			double Solver::load_increment_initial(double dp0)
			{
				return m_dp0 = dp0;
			}

			//time
			double Solver::time_min(void) const
			{
				return m_t_min;
			}
			double Solver::time_min(double t_min)
			{
				return m_t_min = t_min;
			}

			double Solver::time_max(void) const
			{
				return m_t_max;
			}
			double Solver::time_max(double t_max)
			{
				return m_t_max = t_max;
			}

			double Solver::time_initial(double t0)
			{
				return m_t0 = t0;
			}
			double Solver::time_initial(void) const
			{
				return m_t0;
			}

			double Solver::time_increment_min(void) const
			{
				return m_dt_min;
			}
			double Solver::time_increment_min(double dt_min)
			{
				return m_dt_min = dt_min;
			}

			double Solver::time_increment_max(void) const
			{
				return m_dt_max;
			}
			double Solver::time_increment_max(double dt_max)
			{
				return m_dt_max = dt_max;
			}

			double Solver::time_increment_initial(void) const
			{
				return m_dt0;
			}
			double Solver::time_increment_initial(double dt0)
			{
				return m_dt0 = dt0;
			}

			//name
			const char* Solver::type_name(void) const
			{
				return type_name(type());
			}
			const char* Solver::type_name(Type type)
			{
				const char* names[] = {
					"Drift", "Modal", "State", "Buckling", "Tensegrity",
					"Static Linear", "Dynamic Linear", "Transient Linear", "Static Nonlinear", "Dynamic Nonlinear", "Transient Nonlinear"
				};
				return names[math::bit_index(uint32_t(type))];
			}
			const char* Solver::parameter(void) const
			{
				const char* names[] = {
					"", "Frequency", "Time", "Stiffness", "", "Load", "Time", "Time", "Load", "Time", "Time"
				};
				return names[math::bit_index(uint32_t(type()))];
			}

			//results
			double Solver::state_data(uint32_t step) const
			{
				return m_state_data[step];
			}
			double Solver::energy_data(uint32_t type, uint32_t step) const
			{
				return m_energy_data[2 * step + type];
			}

			//solve
			bool Solver::solve(void)
			{
				return true;
			}
			void Solver::print(void)
			{
				printf("step: %d\n", m_step);
			}
			void Solver::record(void)
			{
				//data
				m_assembler->assemble_kinetic_energy();
				m_assembler->assemble_internal_energy();
				m_state_data[m_step] = state_set() & uint32_t(state::p) ? m_p_new : m_t_new;
				//rotations
				const uint32_t nu = m_assembler->m_dof_unknow;
				const uint32_t t1 = uint32_t(mesh::nodes::dof::rotation_1);
				for(const mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
				{
					if(!node->m_quat_new) continue;
					const uint32_t index = math::bit_index(node->m_dof_set, t1);
					for(uint32_t i = 0; i < 3; i++)
					{
						if(node->m_dof_index[index + i] < nu)
						{
							m_x_old[node->m_dof_index[index + i]] = 0;
							m_x_new[node->m_dof_index[index + i]] = 0;
						}
					}
				}
			}
			void Solver::update(void)
			{
				//data
				const uint32_t ss = state_set();
				const uint32_t nu = m_assembler->m_dof_unknow;
				//update
				if(ss & uint32_t(state::t)) m_t_old = m_t_new;
				if(ss & uint32_t(state::p)) m_p_old = m_p_new;
				if(ss & uint32_t(state::x)) memcpy(m_x_old, m_x_new, nu * sizeof(double));
				if(ss & uint32_t(state::v)) memcpy(m_v_old, m_v_new, nu * sizeof(double));
				if(ss & uint32_t(state::a)) memcpy(m_a_old, m_a_new, nu * sizeof(double));
			}
			void Solver::restore(void)
			{
				//data
				const uint32_t ss = state_set();
				const uint32_t nu = m_assembler->m_dof_unknow;
				//update
				if(ss & uint32_t(state::t)) m_t_new = m_t_old;
				if(ss & uint32_t(state::p)) m_p_new = m_p_old;
				if(ss & uint32_t(state::x)) memcpy(m_x_new, m_x_old, nu * sizeof(double));
				if(ss & uint32_t(state::v)) memcpy(m_v_new, m_v_old, nu * sizeof(double));
				if(ss & uint32_t(state::a)) memcpy(m_a_new, m_a_old, nu * sizeof(double));
			}

			void Solver::setup(void)
			{
				//data
				m_step = 0;
				m_stop = false;
				m_t_old = m_t_new = m_t0;
				m_p_old = m_p_new = m_p0;
				m_dp = std::min(m_dp0, m_dp_max);
				m_dt = std::min(m_dt0 != 0 ? m_dt0 : (m_t_max - m_t_min) / m_step_max, m_dt_max);
				//state
				delete[] m_state_data;
				delete[] m_energy_data;
				m_state_data = new double[m_step_max + 1];
				m_energy_data = new double[2 * (m_step_max + 1)];
				//watch dof
				m_watch_dof.setup();
			}
			void Solver::cleanup(void)
			{
				double** data[] = {
					&m_r, &m_R, 
					&m_fnu, &m_fnk, 
					&m_fiu, &m_fik, 
					&m_feu, &m_fek, 
					&m_fru, &m_frk, 
					&m_fdu, &m_fdk, 
					&m_K, &m_C, &m_M, &m_S, 
					&m_k, &m_m, &m_z, &m_e, 
					&m_dxr, &m_dxt, &m_dvt, 
					&m_ddx, &m_ddxr, &m_ddxt, 
					&m_x_old, &m_x_new, &m_dx, 
					&m_v_old, &m_v_new, &m_dv, 
					&m_a_old, &m_a_new, &m_da,
					&m_state_data, &m_energy_data
				};
				for(double** ptr : data)
				{
					delete[] *ptr;
					*ptr = nullptr;
				}
			}
			bool Solver::checkup(void) const
			{
				//data
				const uint32_t ns = m_analysis->model()->boundary()->load_combinations().size();
				//check combination
				if(m_combination != UINT_MAX && m_combination >= ns)
				{
					printf("The selected combination is out of range!\n");
					return false;
				}
				//check time limits
				if(m_t_max < m_t_min || m_dt_max < 0)
				{
					printf("The time limits are inconsistent!\n");
					return false;
				}
				//return
				return m_watch_dof.checkup();
			}

			//compute
			void Solver::compute_residue(void)
			{
				//data
				const uint32_t fs = force_set();
				const uint32_t nu = m_assembler->m_dof_unknow;
				//compute
				for(uint32_t i = 0; i < nu; i++)
				{
					m_r[i] = 0;
					m_r[i] -= fs & uint32_t(force::fn) ? m_fnu[i] : 0;
					m_r[i] -= fs & uint32_t(force::fi) ? m_fiu[i] : 0;
					m_r[i] += fs & uint32_t(force::fe) ? m_feu[i] : 0;
					m_r[i] += fs & uint32_t(force::fd) ? m_fdu[i] : 0;
					m_r[i] += fs & uint32_t(force::fr) ? m_p_new * m_fru[i] : 0;
				}
			}
			void Solver::compute_reactions(void)
			{
				//data
				const uint32_t fs = force_set();
				const uint32_t nk = m_assembler->m_dof_know;
				//compute
				for(uint32_t i = 0; i < nk; i++)
				{
					m_R[i] = 0;
					m_R[i] += fs & uint32_t(force::fn) ? m_fnk[i] : 0;
					m_R[i] += fs & uint32_t(force::fi) ? m_fik[i] : 0;
					m_R[i] -= fs & uint32_t(force::fe) ? m_fek[i] : 0;
					m_R[i] -= fs & uint32_t(force::fd) ? m_fdk[i] : 0;
					m_R[i] -= fs & uint32_t(force::fr) ? m_p_new * m_frk[i] : 0;
				}
			}

			//allocation
			void Solver::allocate_data(void)
			{
				m_state_data = new double[m_step_max + 1];
				m_energy_data = new double[2 * (m_step_max + 1)];
			}
			void Solver::allocate_state(uint32_t nu)
			{
				if(state_set() & (uint32_t) solvers::state::x)
				{
					m_dx = new double[nu];
					m_dxr = new double[nu];
					m_dxt = new double[nu];
					m_ddx = new double[nu];
					m_ddxr = new double[nu];
					m_ddxt = new double[nu];
					m_x_old = new double[nu];
					m_x_new = new double[nu];
					memset(m_x_old, 0, nu * sizeof(double));
					memset(m_x_new, 0, nu * sizeof(double));
				}
			}
			void Solver::allocate_velocity(uint32_t nu)
			{
				if(state_set() & (uint32_t) solvers::state::v)
				{
					m_dv = new double[nu];
					m_dvt = new double[nu];
					m_v_old = new double[nu];
					m_v_new = new double[nu];
					memset(m_v_old, 0, nu * sizeof(double));
					memset(m_v_new, 0, nu * sizeof(double));
				}
			}
			void Solver::allocate_acceleration(uint32_t nu)
			{
				if(state_set() & (uint32_t) solvers::state::a)
				{
					m_da = new double[nu];
					m_a_old = new double[nu];
					m_a_new = new double[nu];
					memset(m_a_old, 0, nu * sizeof(double));
					memset(m_a_new, 0, nu * sizeof(double));
				}
			}
			void Solver::allocate_forces(uint32_t nu, uint32_t nk)
			{
				const uint32_t sf = force_set();
				if(sf & (uint32_t) solvers::force::r) m_r = new double[nu];
				if(sf & (uint32_t) solvers::force::R) m_R = new double[nk];
				if(sf & (uint32_t) solvers::force::fn) m_fnu = new double[nu];
				if(sf & (uint32_t) solvers::force::fn) m_fnk = new double[nk];
				if(sf & (uint32_t) solvers::force::fi) m_fiu = new double[nu];
				if(sf & (uint32_t) solvers::force::fi) m_fik = new double[nk];
				if(sf & (uint32_t) solvers::force::fe) m_feu = new double[nu];
				if(sf & (uint32_t) solvers::force::fe) m_fek = new double[nk];
				if(sf & (uint32_t) solvers::force::fr) m_fru = new double[nu];
				if(sf & (uint32_t) solvers::force::fr) m_frk = new double[nk];
				if(sf & (uint32_t) solvers::force::fd) m_fdu = new double[nu];
				if(sf & (uint32_t) solvers::force::fd) m_fdk = new double[nk];
			}
			void Solver::allocate_tangents(uint32_t nu, uint32_t nz)
			{
				const uint32_t st = tangent_set();
				if(st & (uint32_t) solvers::tangent::K) m_K = new double[nz];
				if(st & (uint32_t) solvers::tangent::C) m_C = new double[nz];
				if(st & (uint32_t) solvers::tangent::M) m_M = new double[nz];
				if(st & (uint32_t) solvers::tangent::S) m_S = new double[nz];
				if(st & (uint32_t) solvers::tangent::Z) m_e = new double[nu];
				if(st & (uint32_t) solvers::tangent::Z) m_z = new double[nu * nu];
				if(st & (uint32_t) solvers::tangent::Z) m_k = new double[nu * nu];
				if(st & (uint32_t) solvers::tangent::Z) m_m = new double[nu * nu];
			}
			void Solver::allocate(uint32_t nu, uint32_t nk, uint32_t nz)
			{
				//data
				allocate_data();
				//state
				allocate_state(nu);
				allocate_velocity(nu);
				allocate_acceleration(nu);
				//forces and tangents
				allocate_forces(nu, nk);
				allocate_tangents(nu, nz);
			}

			//linear
			void Solver::linear_delete(void)
			{
				umfpack_di_free_numeric(&m_num);
				umfpack_di_free_symbolic(&m_sym);
			}
			bool Solver::linear_decompose(const double* K)
			{
				//data
				const int32_t* c = m_assembler->m_cols_map;
				const int32_t* r = m_assembler->m_rows_map;
				const uint32_t n = m_assembler->m_dof_unknow;
				//decompose
				if(!umfpack_di_symbolic(n, n, c, r, K, &m_sym, nullptr, nullptr) == UMFPACK_OK)
				{
					return false;
				}
				return umfpack_di_numeric(c, r, K, m_sym, &m_num, nullptr, nullptr) == UMFPACK_OK;
			}

			bool Solver::linear_substitute(double* x, const double* K, const double* f)
			{
				const int32_t* c = m_assembler->m_cols_map;
				const int32_t* r = m_assembler->m_rows_map;
				return umfpack_di_solve(UMFPACK_A, c, r, K, x, f, m_num, nullptr, nullptr) == UMFPACK_OK;
			}
			bool Solver::linear_solve(double* x, const double* K, const double* f)
			{
				//solve
				bool test = false;
				const int32_t* c = m_assembler->m_cols_map;
				const int32_t* r = m_assembler->m_rows_map;
				const uint32_t n = m_assembler->m_dof_unknow;
				if(umfpack_di_symbolic(n, n, c, r, K, &m_sym, nullptr, nullptr) == UMFPACK_OK)
				{
					if(umfpack_di_numeric(c, r, K, m_sym, &m_num, nullptr, nullptr) == UMFPACK_OK)
					{
						test = umfpack_di_solve(UMFPACK_A, c, r, K, x, f, m_num, nullptr, nullptr) == UMFPACK_OK;
					}
				}
				//free memory
				umfpack_di_free_numeric(&m_num);
				umfpack_di_free_symbolic(&m_sym);
				//return
				return test;
			}

			//static data
			Analysis* Solver::m_analysis = nullptr;
		}
	}
}
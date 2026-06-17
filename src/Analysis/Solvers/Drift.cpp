//std
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstring>

//Math
#include "Math/inc/Miscellaneous/util.hpp"
#include "Math/inc/Linear/Quat.hpp"
#include "Math/inc/Linear/Sparse.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/Drift.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Drift::Drift(void) : m_scale(1e-5), m_threshold(1e-2), m_field(solvers::field::stiffness)
			{
				return;
			}

			//destructor
			Drift::~Drift(void)
			{
				return;
			}

			//serialization
			void Drift::load(FILE* file)
			{
				Solver::load(file);
				Nonlinear::load(file);
			}
			void Drift::save(FILE* file) const
			{
				Solver::save(file);
				Nonlinear::save(file);
			}

			//type
			Type Drift::type(void) const
			{
				return Type::State;
			}

			//sets
			uint32_t Drift::state_set(void) const
			{
				return 
					(uint32_t) solvers::state::x | 
					(uint32_t) solvers::state::t;
			}
			uint32_t Drift::force_set(void) const
			{
				return (uint32_t) solvers::force::fi;
			}
			uint32_t Drift::tangent_set(void) const
			{
				return (m_field == field::stiffness) * (uint32_t) solvers::tangent::K;
			}

			//data
			double Drift::scale(void) const
			{
				return m_scale;
			}
			double Drift::scale(double scale)
			{
				return m_scale = scale;
			}
			double Drift::threshold(void) const
			{
				return m_threshold;
			}
			double Drift::threshold(double threshold)
			{
				return m_threshold = threshold;
			}
			solvers::field Drift::field(void) const
			{
				return m_field;
			}
			solvers::field Drift::field(solvers::field field)
			{
				return m_field = field;
			}

			//analysis
			void Drift::setup(void)
			{
				Solver::setup();
				Nonlinear::setup();
			}
			bool Drift::solve(void)
			{
				//solve
				srand(::time(nullptr));
				printf("Drift test: %s\n", m_field == solvers::field::force ? "force" : "stiffness");
				m_field == solvers::field::force ? solve_force() : solve_stiffness();
				//return
				return true;
			}

			void Drift::solve_force(void)
			{
				//data
				const uint32_t nd = m_assembler->dof_unknow();
				double* fn = (double*) alloca(nd * sizeof(double));
				double* fe = (double*) alloca(nd * sizeof(double));
				//solve
				while(m_step < m_step_max)
				{
					//state
					random_state();
					m_assembler->apply();
					m_assembler->compute();
					m_assembler->assemble_internal_force();
					math::ndiff(Drift::drift_energy, fn, m_x_new, nullptr, 1, nd, m_scale);
					//error
					for(uint32_t i = 0; i < nd; i++)
					{
						fe[i] = m_fiu[i] - fn[i];
					}
					const double e = math::Vector(fe, nd).norm();
					const double r = math::Vector(m_fiu, nd).norm();
					//check
					if(e > m_threshold * r)
					{
						math::Vector(m_x_new, nd).print("State:");
						math::Vector(fn, nd).print("Numeric force:");
						math::Vector(fe, nd).print("Residue force:", 1e-5 * r);
						math::Vector(m_fiu, nd).print("Analytic force:");
						break;
					}
					//step
					printf("step: %04d error: %+.2e\n", m_step++, e / r);
				}
			}
			void Drift::solve_stiffness(void)
			{
				//data
				const uint32_t nd = m_assembler->dof_unknow();
				const int32_t* rows_map = m_assembler->rows_map();
				const int32_t* cols_map = m_assembler->cols_map();
				double* Kn = (double*) alloca(nd * nd * sizeof(double));
				double* Ke = (double*) alloca(nd * nd * sizeof(double));
				const math::Sparse K(m_K, rows_map, cols_map, nd, nd);
				//solve
				while(m_step < m_step_max)
				{
					//state
					random_state();
					m_assembler->apply();
					m_assembler->compute();
					m_assembler->assemble_stiffness();
					math::ndiff(Drift::drift_force, Kn, m_x_new, nullptr, nd, nd, m_scale);
					//error
					for(uint32_t i = 0; i < nd; i++)
					{
						for(uint32_t j = 0; j < nd; j++)
						{
							Ke[i + nd * j] = K(i, j) - Kn[i + nd * j];
						}
					}
					const double r = K.norm();
					const double e = math::Matrix(Ke, nd, nd).norm();
					//check
					if(e > m_threshold * r)
					{
						math::Vector(m_x_new, nd).print("State:");
						math::Matrix(Kn, nd, nd).print("Numeric stiffness:");
						math::Matrix(Ke, nd, nd).print("Residue stiffness:", 1e-5 * K.norm());
						K.print("Analytic stiffness:");
						break;
					}
					//step
					printf("step: %04d error: %+.2e\n", m_step++, e / r);
				}
			}

			void Drift::random_state(void)
			{
				//data
				const double v1 = m_watch_dof.value_min();
				const double v2 = m_watch_dof.value_max();
				const uint32_t nd = m_assembler->dof_unknow();
				//state
				for(uint32_t i = 0; i < nd; i++)
				{
					m_x_new[i] = v1 + (v2 - v1) * rand() / RAND_MAX;
				}
				for(mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
				{
					if(node->m_quat_old)
					{
						math::Quat(node->m_quat_old).randu();
					}
				}
			}

			void Drift::drift_force(double* f, const double* d, void**)
			{
				//data
				const uint32_t nd = m_analysis->assembler()->dof_unknow();
				Drift* solver = dynamic_cast<Drift*>(m_analysis->solver());
				//state
				memcpy(solver->m_x_new, d, nd * sizeof(double));
				//apply
				m_analysis->assembler()->apply();
				m_analysis->assembler()->compute();
				m_analysis->assembler()->assemble_internal_force();
				//return
				memcpy(f, solver->m_fiu, nd * sizeof(double));
			}
			void Drift::drift_energy(double* U, const double* d, void**)
			{
				//data
				const uint32_t nd = m_analysis->assembler()->dof_unknow();
				Drift* solver = dynamic_cast<Drift*>(m_analysis->solver());
				//state
				memcpy(solver->m_x_new, d, nd * sizeof(double));
				//energy
				m_analysis->assembler()->apply();
				m_analysis->assembler()->compute();
				m_analysis->assembler()->assemble_internal_energy();
				//return
				U[0] = solver->m_energy_data[2 * solver->m_step + 1];
			}
		}
	}
}
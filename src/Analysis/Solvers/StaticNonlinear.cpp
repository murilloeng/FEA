//std
#include <cmath>
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/util.hpp"
#include "Math/inc/Linear/Vector.hpp"
#include "Math/inc/Linear/Sparse.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			StaticNonlinear::StaticNonlinear(void) :
				m_stability{false}, m_frequencies{false}, m_load_adjust(false), m_load_factor(1)
			{
				return;
			}

			//destructor
			StaticNonlinear::~StaticNonlinear(void)
			{
				return;
			}

			//serialization
			void StaticNonlinear::load(FILE* file)
			{
				Eigen::load(file);
				Nonlinear::load(file);
				uint32_t stability, frequencies, load_adjust;
				if(fscanf(file, "%d %d %d", &stability, &frequencies, &load_adjust) != 3)
				{
					throw std::runtime_error("Static Nonlinear solver loading error!");
				}
				m_stability = stability;
				m_frequencies = frequencies;
				m_load_adjust = load_adjust;
			}
			void StaticNonlinear::save(FILE* file) const
			{
				Eigen::save(file);
				Nonlinear::save(file);
				if(fprintf(file, "%d %d %d\n", m_stability, m_frequencies, m_load_adjust) < 0)
				{
					throw std::runtime_error("Static Nonlinear solver saving error!");
				}
			}

			//data
			bool StaticNonlinear::load_adjust(void) const
			{
				return m_load_adjust;
			}
			bool StaticNonlinear::load_adjust(bool load_adjust)
			{
				return m_load_adjust = load_adjust;
			}

			bool StaticNonlinear::stability(void) const
			{
				return m_stability;
			}
			bool StaticNonlinear::stability(bool stability)
			{
				return m_stability = stability;
			}

			bool StaticNonlinear::frequencies(void) const
			{
				return m_frequencies;
			}
			bool StaticNonlinear::frequencies(bool frequencies)
			{
				return m_frequencies = frequencies;
			}

			//type
			Type StaticNonlinear::type(void) const
			{
				return Type::StaticNonlinear;
			}

			//sets
			uint32_t StaticNonlinear::state_set(void) const
			{
				return 
					(uint32_t) solvers::state::x |
					(uint32_t) solvers::state::p |
					(uint32_t) solvers::state::t;
			}
			uint32_t StaticNonlinear::force_set(void) const
			{
				return
					(uint32_t) solvers::force::r |
					(uint32_t) solvers::force::R |
					(uint32_t) solvers::force::fi |
					(uint32_t) solvers::force::fr |
					(uint32_t) solvers::force::fd;
			}
			uint32_t StaticNonlinear::tangent_set(void) const
			{
				return
					(uint32_t) solvers::tangent::K |
					(m_stability || m_frequencies ? (uint32_t) solvers::tangent::M : 0) |
					(m_stability || m_frequencies ? (uint32_t) solvers::tangent::Z : 0);
			}

			//analysis
			void StaticNonlinear::setup(void)
			{
				Solver::setup();
				Nonlinear::setup();
			}
			void StaticNonlinear::print(void)
			{
				//data
				char stability[256];
				strcpy(stability, "");
				const uint32_t nm = modes_count();
				//stability
				if(m_stability)
				{
					bool test = true;
					for(uint32_t i = 0; i < nm; i++) test = test && m_e[i] >= 0;
					sprintf(stability, "stability: %s", test ? "pos def" : "non pos def");
					if(!test)
					{
						for(uint32_t i = 0; i < nm; i++)
						{
							printf("e[%d]: %+.2e\n", i, m_e[i]);
						}
					}
				}
				//print
				static const char* format = "step: %06d attempts: %02d iterations: %02d load: %+.2e dof: %+.2e %s\n";
				printf(format, m_step, m_attempt, m_iteration, m_p_new, m_watch_dof.value(), stability);
			}
			bool StaticNonlinear::solve(void)
			{
				solve_setup();
				while(!m_stop_criteria.compute())
				{
					m_step++;
					m_attempt = 0;
					m_iteration = 0;
					if(m_stop) return true;
					if(m_interface) m_interface();
					while(m_attempt < m_attempt_max)
					{
						if(!solve_predictor()) return true;
						if(!solve_corrector()) return true;
						if(!m_convergence.status()) solve_restore(); else break;
					}
				}
				return true;
			}

			//solve
			void StaticNonlinear::solve_setup(void)
			{
				//assembler
				m_assembler->apply();
				m_assembler->compute();
				m_assembler->assemble_dead_force();
				m_assembler->assemble_internal_force();
				m_assembler->assemble_reference_force();
				//compute
				compute_residue();
				compute_reactions();
				m_stop = m_frequencies && !compute_frequencies();
				//record
				print();
				m_assembler->record();
			}
			void StaticNonlinear::solve_restore(void)
			{
				m_dt /= 2;
				m_attempt++;
				m_iteration = 0;
				m_assembler->restore();
			}
			bool StaticNonlinear::solve_predictor(void)
			{
				//time
				m_t_new = m_t_old + m_dt;
				//assembler
				m_assembler->apply();
				m_assembler->compute();
				m_assembler->assemble_stiffness();
				m_assembler->assemble_dead_force();
				m_assembler->assemble_internal_force();
				m_assembler->assemble_reference_force();
				//compute
				return compute_predictor();
			}
			bool StaticNonlinear::solve_corrector(void)
			{
				while(m_iteration < m_iteration_max)
				{
					//assembler
					m_assembler->apply();
					m_assembler->compute();
					m_assembler->assemble_stiffness();
					m_assembler->assemble_dead_force();
					m_assembler->assemble_internal_force();
					m_assembler->assemble_reference_force();
					//check
					compute_residue();
					if(m_convergence.compute())
					{
						//setup
						compute_reactions();
						compute_load_factor();
						if(m_stability && !compute_stability()) return false;
						if(m_frequencies && !compute_frequencies()) return false;
						//update
						print();
						m_assembler->record();
						m_assembler->update();
						break;
					}
					//update
					m_iteration++;
					if(!compute_corrector()) return false;
				}
				return true;
			}

			//compute
			bool StaticNonlinear::compute_predictor(void)
			{
				//solve
				const bool test = 
					linear_decompose(m_K) && 
					linear_substitute(m_dxr, m_K, m_r) &&
					linear_substitute(m_dxt, m_K, m_fru);
				linear_delete();
				//check
				if(!test)
				{
					printf("\tUnable to compute predictor state increment in step %d\n", m_step);
					return false;
				}
				//continuation
				if(m_step != 1)
				{
					m_dp = m_continuation.predictor() * m_load_factor / (1 << m_attempt);
				}
				if(std::isnan(m_dp))
				{
					printf("\tUnable to compute predictor load increment in step %d\n", m_step);
					return false;
				}
				//increment
				m_dp = fmax(fmin(m_dp, m_dp_max), m_dp_min);
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++) m_dx[i] = m_dxr[i] + m_dp * m_dxt[i];
				//update
				m_p_new = m_p_old + m_dp;
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++) m_x_new[i] = m_x_old[i] + m_dx[i];
				//return
				return true;
			}
			bool StaticNonlinear::compute_corrector(void)
			{
				//solve
				const bool test = 
					linear_decompose(m_K) &&
					linear_substitute(m_ddxr, m_K, m_r) &&
					linear_substitute(m_ddxt, m_K, m_fru);
				linear_delete();
				//check
				if(!test)
				{
					printf("Unable to compute corrector state increment in step %d!\n", m_step);
					return false;
				}
				//continuation
				m_ddp = m_continuation.corrector();
				if(std::isnan(m_ddp))
				{
					printf("\tUnable to compute corrector load increment in step %d\n", m_step);
					return false;
				}
				//increment
				m_dp += m_ddp;
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++) m_dx[i] += m_ddx[i] = m_ddxr[i] + m_ddp * m_ddxt[i];
				//update
				m_p_new = m_p_old + m_dp;
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++) m_x_new[i] = m_x_old[i] + m_dx[i];
				//return
				return true;
			}
			bool StaticNonlinear::compute_stability(void)
			{
				//assemble
				m_assembler->assemble_stiffness();
				m_assembler->assemble_dead_force();
				m_assembler->assemble_internal_force();
				m_assembler->assemble_reference_force();
				//return
				return eigen_std();
			}
			bool StaticNonlinear::compute_frequencies(void)
			{
				//assemble
				m_assembler->assemble_inertia();
				m_assembler->assemble_stiffness();
				m_assembler->assemble_dead_force();
				m_assembler->assemble_internal_force();
				m_assembler->assemble_reference_force();
				//return
				return eigen_gen();
			}
			void StaticNonlinear::compute_load_factor(void)
			{
				const uint32_t ni = m_iteration + 1;
				const uint32_t nd = m_iteration_desired + 1;
				m_load_factor = m_load_adjust ? sqrt(double(nd) / ni) : 1;
			}
		}
	}
}
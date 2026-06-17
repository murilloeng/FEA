//std
#include <cmath>
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Sparse.hpp"
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicNonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			DynamicNonlinear::DynamicNonlinear(void)
			{
				return;
			}

			//destructor
			DynamicNonlinear::~DynamicNonlinear(void)
			{
				return;
			}

			//serialization
			void DynamicNonlinear::load(FILE* file)
			{
				Solver::load(file);
				Dynamic::load(file);
				Nonlinear::load(file);
			}
			void DynamicNonlinear::save(FILE* file) const
			{
				Solver::save(file);
				Dynamic::save(file);
				Nonlinear::save(file);
			}

			//type
			Type DynamicNonlinear::type(void) const
			{
				return Type::DynamicNonlinear;
			}

			//analysis
			void DynamicNonlinear::setup(void)
			{
				Solver::setup();
				Nonlinear::setup();
			}

			void DynamicNonlinear::print_step(void) const
			{
				if(m_integration == Integration::runge_kutta)
				{
					printf("step: %06d time: %+.2e dof: %+.2e\n", m_step, m_t_new, m_watch_dof.value());
				}
				else if(m_integration == Integration::newmark)
				{
					printf("step: %06d time: %+.2e dof: %+.2e attempt: %02d iterations: %02d\n",
						m_step, m_t_new, m_watch_dof.value(), m_attempt, m_iteration);
				}
			}

			bool DynamicNonlinear::solve_newmark(void)
			{
				while(!m_stop_criteria.compute())
				{
					//iterations
					compute_predictor();
					if(m_stop) return false;
					if(m_interface) m_interface();
					while(m_iteration < m_iteration_max)
					{
						//equilibrium
						compute_residue();
						if(m_convergence.compute())
						{
							compute_update();
							break;
						}
						//update
						compute_tangent();
						compute_corrector();
						m_assembler->compute();
						m_assembler->assemble();
						//increment
						m_iteration++;
					}
					//check solution
					if(!m_convergence.status())
					{
						compute_restore();
						continue;
					}
					//time adjust
					compute_time();
				}
				return m_convergence.status();
			}
			bool DynamicNonlinear::solve_runge_kutta(void)
			{
				while(m_step < m_step_max)
				{
					//step
					m_step++;
					if(m_stop) return false;
					if(m_interface) m_interface();
					//incremet
					compute_tangent_1();
					if(!compute_tangent_2()) return false;
					if(!compute_tangent_3()) return false;
					if(!compute_tangent_4()) return false;
					for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++)
					{
						m_x_new[i] = m_x_old[i] + m_dx[i];
						m_v_new[i] = m_v_old[i] + m_dv[i];
					}
					//assembler
					m_assembler->apply();
					m_assembler->compute();
					m_assembler->assemble();
					//solve
					if(!compute_acceleration())
					{
						return false;
					}
					m_assembler->apply_acceleration();
					m_assembler->assemble_inertial_force();
					//assembler
					print_step();
					compute_reactions();
					m_assembler->record();
					m_assembler->update();
				}
				//return
				return true;
			}

			void DynamicNonlinear::compute_time(void)
			{
				if(m_time_adjust)
				{
					m_dt *= sqrt(double(m_iteration_desired + 1) / (m_iteration + 1));
				}
			}
			void DynamicNonlinear::compute_update(void)
			{
				print_step();
				m_attempt = 0;
				m_iteration = 0;
				compute_reactions();
				m_assembler->record();
				m_assembler->update();
			}
			void DynamicNonlinear::compute_restore(void)
			{
				m_dt /= 2;
				m_attempt++;
				m_iteration = 0;
				m_assembler->restore();
				m_t_new = m_t_old + m_dt;
				printf("\tStep %i failed in attempt %i!\n", m_step, m_attempt);
			}
			void DynamicNonlinear::compute_tangent(void)
			{
				double a, b, g;
				m_newmark.parameters(a, b, g);
				m_assembler->assemble_inertia();
				m_assembler->assemble_damping();
				m_assembler->assemble_stiffness();
				for(uint32_t i = 0; i < m_assembler->dof_nonzero(); i++)
				{
					m_S[i] = m_M[i] + g * m_dt * m_C[i] + b * m_dt * m_dt * m_K[i];
				}
			}
			void DynamicNonlinear::compute_tangent_1(void)
			{
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++)
				{
					m_dx[i] = m_dt / 6 * m_v_new[i];
					m_dv[i] = m_dt / 6 * m_a_new[i];
				}
			}
			bool DynamicNonlinear::compute_tangent_2(void)
			{
				//data
				const uint32_t nu = m_assembler->dof_unknow();
				//state
				m_t_new = m_t_old + m_dt / 2;
				for(uint32_t i = 0; i < nu; i++)
				{
					m_x_new[i] = m_x_old[i] + m_dt / 2 * m_v_new[i];
					m_v_new[i] = m_v_old[i] + m_dt / 2 * m_a_new[i];
				}
				//assembler
				m_assembler->apply();
				m_assembler->compute();
				m_assembler->assemble();
				math::Vector(m_feu, nu) -= math::Vector(m_fiu, nu);
				//solve
				if(!linear_solve(m_a_new, m_M, m_feu))
				{
					printf("Unable to compute 2nd order tangent in step %d!\n",m_step);
					return false;
				}
				//increment
				for(uint32_t i = 0; i < nu; i++)
				{
					m_dx[i] += m_dt / 3 * m_v_new[i];
					m_dv[i] += m_dt / 3 * m_a_new[i];
				}
				//return
				return true;
			}
			bool DynamicNonlinear::compute_tangent_3(void)
			{
				//data
				const uint32_t nu = m_assembler->dof_unknow();
				//state
				for(uint32_t i = 0; i < nu; i++)
				{
					m_x_new[i] = m_x_old[i] + m_dt / 2 * m_v_new[i];
					m_v_new[i] = m_v_old[i] + m_dt / 2 * m_a_new[i];
				}
				//assembler
				m_assembler->apply();
				m_assembler->compute();
				m_assembler->assemble();
				math::Vector(m_feu, nu) -= math::Vector(m_fiu, nu);
				//solve
				if(!linear_solve(m_a_new, m_M, m_feu))
				{
					printf("Unable to compute 3rd order tangent in step %d!\n",m_step);
					return false;
				}
				//increment
				for(uint32_t i = 0; i < nu; i++)
				{
					m_dx[i] += m_dt / 3 * m_v_new[i];
					m_dv[i] += m_dt / 3 * m_a_new[i];
				}
				//return
				return true;
			}
			bool DynamicNonlinear::compute_tangent_4(void)
			{
				//data
				const uint32_t nu = m_assembler->dof_unknow();
				//update state
				m_t_new = m_t_old + m_dt;
				for(uint32_t i = 0; i < nu; i++)
				{
					m_x_new[i] = m_x_old[i] + m_dt * m_v_new[i];
					m_v_new[i] = m_v_old[i] + m_dt * m_a_new[i];
				}
				//assembler
				m_assembler->apply();
				m_assembler->compute();
				m_assembler->assemble();
				math::Vector(m_feu, nu) -= math::Vector(m_fiu, nu);
				//solve
				if(!linear_solve(m_a_new, m_M, m_feu))
				{
					printf("Unable to compute 4th order tangent in step %d!\n",m_step);
					return false;
				}
				//update state
				for(uint32_t i = 0; i < nu; i++)
				{
					m_dx[i] += m_dt / 6 * m_v_new[i];
					m_dv[i] += m_dt / 6 * m_a_new[i];
				}
				//return
				return true;
			}
			void DynamicNonlinear::compute_predictor(void)
			{
				if(!m_attempt) m_step++;
				if(!m_attempt) m_t_new = m_t_old + m_dt;
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++)
				{
					m_v_new[i] = m_v_old[i] + m_dt * m_a_old[i];
					m_x_new[i] = m_x_old[i] + m_dt * m_v_old[i] + m_dt * m_dt / 2 * m_a_old[i];
				}
				m_assembler->apply();
				m_assembler->compute();
				m_assembler->assemble();
			}
			bool DynamicNonlinear::compute_corrector(void)
			{
				//corrector
				double a, b, g;
				m_newmark.parameters(a, b, g);
				if(!linear_solve(m_da, m_S, m_r))
				{
					printf("Unable to compute dof corrector in step %d!\n", m_step);
					return false;
				}
				//update
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++)
				{
					m_a_new[i] += m_da[i];
					m_v_new[i] += g * m_dt * m_da[i];
					m_x_new[i] += b * m_dt * m_dt * m_da[i];
				}
				m_assembler->apply_state();
				m_assembler->apply_velocity();
				m_assembler->apply_acceleration();
				//return
				return true;
			}
		}
	}
}
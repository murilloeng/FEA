//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicLinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			DynamicLinear::DynamicLinear(void)
			{
				return;
			}

			//destructor
			DynamicLinear::~DynamicLinear(void)
			{
				return;
			}

			//serialization
			void DynamicLinear::load(FILE* file)
			{
				Solver::load(file), Dynamic::load(file);
			}
			void DynamicLinear::save(FILE* file) const
			{
				Solver::save(file), Dynamic::save(file);
			}

			//type
			Type DynamicLinear::type(void) const
			{
				return Type::DynamicLinear;
			}

			//analysis
			void DynamicLinear::setup(void)
			{
				Solver::setup();
			}

			bool DynamicLinear::solve_newmark(void)
			{
				compute_tangent();
				while(!m_stop_criteria.compute())
				{
					//time
					m_step++;
					m_t_new += m_dt;
					if(m_stop) return false;
					if(m_interface) m_interface();
					//predictor
					compute_predictor();
					m_assembler->apply();
					m_assembler->compute();
					m_assembler->assemble_external_force();
					m_assembler->assemble_internal_force();
					//corector
					if(!compute_corrector())
					{
						return false;
					}
					//assembler
					m_assembler->apply();
					m_assembler->compute();
					m_assembler->assemble_internal_force();
					m_assembler->assemble_inertial_force();
					//record
					compute_reactions();
					m_assembler->record();
					m_assembler->update();
					//print
					printf("step: %04d time: %.2e dof: %+.2e\n", m_step, m_t_new, m_watch_dof.value());
				}
				return true;
			}
			bool DynamicLinear::solve_runge_kutta(void)
			{
				return false;
			}

			void DynamicLinear::compute_tangent(void)
			{
				double a, b, g;
				m_newmark.parameters(a, b, g);
				for(uint32_t i = 0; i < m_assembler->dof_nonzero(); i++)
				{
					m_S[i] = m_M[i] + g * m_dt * m_C[i] + b * m_dt * m_dt * m_K[i];
				}
				linear_decompose(m_S);
			}
			void DynamicLinear::compute_predictor(void)
			{
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++)
				{
					m_v_new[i] = m_v_old[i] + m_dt * m_a_old[i];
					m_x_new[i] = m_x_old[i] + m_dt * m_v_old[i] + m_dt * m_dt / 2 * m_a_old[i];
				}
			}
			bool DynamicLinear::compute_corrector(void)
			{
				double a, b, g;
				m_newmark.parameters(a, b, g);
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++)
				{
					m_r[i] = m_feu[i] - m_fiu[i] - m_fnu[i];
				}
				if(!linear_substitute(m_da, m_S, m_r))
				{
					printf("Unable to compute corrector in step %d!\n", m_step);
					return false;
				}
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++)
				{
					m_a_new[i] += m_da[i];
					m_v_new[i] += g * m_dt * m_da[i];
					m_x_new[i] += b * m_dt * m_dt * m_da[i];
				}
				return true;
			}
		}
	}
}
//std
#include <cstring>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/State.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			State::State(void)
			{
				return;
			}

			//destructor
			State::~State(void)
			{
				return;
			}

			//serialization
			void State::load(FILE* file)
			{
				Solver::load(file);
				Nonlinear::load(file);
			}
			void State::save(FILE* file) const
			{
				Solver::save(file);
				Nonlinear::save(file);
			}

			//type
			Type State::type(void) const
			{
				return Type::State;
			}

			//sets
			uint32_t State::state_set(void) const
			{
				return 
					(uint32_t) solvers::state::x |
					(uint32_t) solvers::state::t;
			}
			uint32_t State::force_set(void) const
			{
				return
					(uint32_t) solvers::force::R |
					(uint32_t) solvers::force::fi;
			}
			uint32_t State::tangent_set(void) const
			{
				return 0;
			}

			//analysis
			void State::setup(void)
			{
				Solver::setup();
				Nonlinear::setup();
			}
			bool State::solve(void)
			{
				//apply configuration
				m_assembler->apply_supports();
				m_assembler->compute();
				//record
				m_assembler->record();
				printf("step: %04d dof: %+.2e\n", m_step, m_watch_dof.value());
				//incremental loop
				while(m_step < m_step_max)
				{
					//increment
					m_t_new += m_dt;
					//apply configuration
					m_assembler->apply_supports();
					m_assembler->compute();
					//compute internal force
					m_assembler->assemble_internal_force();
					//print
					m_step++;
					printf("step: %04d dof: %+.2e\n", m_step, m_watch_dof.value());
					//record and update
					compute_reactions();
					m_assembler->record();
					m_assembler->update();
				}
				//return
				return true;
			}

			void State::compute_reactions(void)
			{
				memcpy(m_R, m_fik, m_assembler->dof_know() * sizeof(double));
			}
		}
	}
}
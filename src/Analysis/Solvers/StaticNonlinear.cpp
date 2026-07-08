//std
#include <cstdio>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		StaticNonlinear::StaticNonlinear(void)
		{
			return;
		}
		
		//destructor
		StaticNonlinear::~StaticNonlinear(void)
		{
			return;
		}

		//analysis
		void StaticNonlinear::check(void)
		{
			Solver::check();
		}
		void StaticNonlinear::setup(void)
		{
			//setup
			Solver::setup();
			math::solvers::Implicit::setup();
			math::solvers::Incremental::setup();
			//system
			m_system_2 = [this](double* r, double* fe, double* K, double p, const double* x)
			{
				m_analysis->m_model->compute();
				m_analysis->m_assembler->assemble_stiffness(K);
				m_analysis->m_assembler->assemble_dead_force(m_r, true);
				m_analysis->m_assembler->assemble_internal_force(m_r, false, -1);
				m_analysis->m_assembler->assemble_reference_force(m_r, false, p);
				m_analysis->m_assembler->assemble_reference_force(m_fe, true, 1);
			};
		}
		void StaticNonlinear::print(void)
		{
			if(m_silent) return;
			printf("Step: %4d Attempts: %d Iterations: %d Load: %+.6e State: %+.6e\n", m_step, m_attempt, m_iteration, m_p_new, m_watch_dof.state());
		}

		//solve
		void StaticNonlinear::solve(void)
		{
			print();
			record();
			compute();
			for(m_step = 1; !stop(); m_step++)
			{
				step();
				print();
				if(!m_status)
				{
					if(!m_silent) printf("Solver failed in step %d!\n", m_step);
					break;
				}
				if(m_callback_step) m_callback_step();
			}
		}
	}
}
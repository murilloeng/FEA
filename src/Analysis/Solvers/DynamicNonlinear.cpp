//std
#include <cstdio>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicNonlinear.hpp"

//target system: r(x, v, a, t) = fe(x, t) - fi(x, v, a) = 0

//fi: internal force including inertial, damping and constraint (lagrange multipliers in a) contributions

namespace fea
{
	namespace analysis
	{
		//analysis
		void DynamicNonlinear::check(void)
		{
			Solver::check();
		}
		void DynamicNonlinear::setup(void)
		{
			//setup
			Solver::setup();
			//system
			m_analysis->assembler()->request().m_U = true;
			m_analysis->assembler()->request().m_V = true;
			m_analysis->assembler()->request().m_K = true;
			m_analysis->assembler()->request().m_C = true;
			m_analysis->assembler()->request().m_M = true;
			m_analysis->assembler()->request().m_f = true;
			m_analysis->assembler()->request().m_time = true;
			m_system = [this](double*, double*, double*, double*, double*, const double*, const double*, const double*, double)
			{
				reset();
				m_analysis->assembler()->assemble();
			};
			//callbacks
			m_callback_update = [this](void){ m_analysis->model()->update(); };
			m_callback_restore = [this](void){ m_analysis->model()->restore(); };
			//newmark
			math::solvers::Newmark::setup();
		}
		void DynamicNonlinear::print(void)
		{
			if(m_silent) return;
			printf("Step: %4d Attempts: %d Iterations: %d Time: %+.6e State: %+.6e\n", m_step, m_attempt, m_iteration, m_t_new, m_watch_dof.state());
		}
		void DynamicNonlinear::compute(void)
		{
			m_system(m_fi, m_fe, m_K, m_C, m_M, m_x_new, m_v_new, m_a_new, m_t_new);
			for(uint32_t i = 0; i < m_size; i++) m_r[i] = m_fe[i] - m_fi[i];
		}

		//solve
		void DynamicNonlinear::solve(void)
		{
			print();
			record();
			compute();
			for(m_step = 1; !stop(); m_step++)
			{
				compute_step();
				if(!m_status)
				{
					m_status = true;
					if(!m_silent) printf("Solver failed in step %d!\n", m_step);
					break;
				}
				print();
				if(m_callback_step) m_callback_step();
			}
		}
	}
}
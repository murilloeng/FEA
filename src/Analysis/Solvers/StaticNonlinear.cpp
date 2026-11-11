//std
#include <cstdio>
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		//data
		uint32_t StaticNonlinear::forces_set(void) const
		{
			return 
				1 << uint32_t(math::solvers::Solver::Force::r)|
				1 << uint32_t(math::solvers::Solver::Force::fi)|
				1 << uint32_t(math::solvers::Solver::Force::fe)|
				1 << uint32_t(math::solvers::Solver::Force::fd);
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
			//eigen
			const uint32_t nv = m_eigen_std.vectors();
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			m_eigen_std.setup(nu, 1, nv, m_rows_map, m_cols_map, m_K, m_eigenvalues, m_eigenvectors);
			//system
			m_analysis->assembler()->request().m_U = true;
			m_analysis->assembler()->request().m_V = true;
			m_analysis->assembler()->request().m_K = true;
			m_analysis->assembler()->request().m_fi = true;
			m_system = [this](double*, double*, double*, const double*, double)
			{
				reset();
				m_analysis->assembler()->assemble();
				for(uint32_t i = 0; i < m_size; i++) m_r[i] = m_p_new * m_fe[i] + m_fd[i] - m_fi[i];
			};
			//callbacks
			m_callback_update = [this](void){ m_analysis->model()->update(); };
			m_callback_restore = [this](void){ m_analysis->model()->restore(); };
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
				compute_step();
				if(!m_status)
				{
					m_status = true;
					if(!m_silent) printf("Solver failed in step %d!\n", m_step);
					break;
				}
				print();
				if(m_stability) compute_stability();
				if(m_callback_step) m_callback_step();
			}
		}

		//compute
		void StaticNonlinear::compute_stability(void)
		{
			//compute
			if(!m_eigen_std.compute())
			{
				throw std::runtime_error("Error: Static Nonlinear solver stability computation failed!");
			}
			//stability
			m_stability_data[m_step] = m_eigenvalues[0] > 0;
		}
	}
}
//std
#include <cstdio>
#include <stdexcept>

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
			//eigen
			const uint32_t nv = m_eigen_std.vectors();
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			m_eigen_std.setup(nu, 1, nv, m_rows_map, m_cols_map, m_K, m_s, m_U);
			//system
			m_system = [this](double* r, double* fe, double* K, double p, const double* x)
			{
				m_analysis->model()->compute();
				m_analysis->assembler()->assemble_stiffness(K);
				m_analysis->assembler()->assemble_dead_force(r, true);
				m_analysis->assembler()->assemble_internal_force(r, false, -1);
				m_analysis->assembler()->assemble_reference_force(r, false, p);
				m_analysis->assembler()->assemble_reference_force(fe, true, 1);
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
				compute_step();
				if(!m_status)
				{
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
			m_stability_data[m_step] = m_s[0] > 0;
		}
	}
}
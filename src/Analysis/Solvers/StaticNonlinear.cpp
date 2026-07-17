//std
#include <cstdio>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Eigen.hpp"

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
		StaticNonlinear::StaticNonlinear(void) : m_stability{false}, m_stability_data{nullptr}
		{
			return;
		}

		//destructor
		StaticNonlinear::~StaticNonlinear(void)
		{
			delete[] m_stability_data;
		}

		//data
		bool StaticNonlinear::stability(void) const
		{
			return m_stability;
		}
		bool StaticNonlinear::stability(bool stability)
		{
			return m_stability = stability;
		}

		bool StaticNonlinear::stability_data(uint32_t step) const
		{
			return m_stability_data[step];
		}

		//save
		void StaticNonlinear::save(const char* path, std::vector<std::pair<uint32_t, mesh::nodes::DOF>> list) const
		{
			Incremental::save(path, list);
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
			//stability
			if(m_stability)
			{
				delete[] m_stability_data;
				m_stability_data = new bool[m_step_max + 1];
				for(uint32_t i = 0; i < m_step_max + 1; i++) m_stability_data[i] = true;
			}
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
			//data
			double s;
			double* z = new double[m_size];
			//compute
			if(!math::Eigen(m_K, m_size, m_rows_map, m_cols_map, &s, z, 1, std::min(m_size, 5U)).compute())
			{
				throw std::runtime_error("Error: StaticNonlinear stability computation failed!");
			}
			//stability
			m_stability_data[m_step] = s > 0;
			//delete
			delete[] z;
		}
	}
}
//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Solver::Solver(void) : m_load_combination{UINT32_MAX}
		{
			return;
		}
		
		//destructor
		Solver::~Solver(void)
		{
			return;
		}

		//analysis
		void Solver::check(void)
		{
			m_watch_dof.check();
		}
		void Solver::setup(void)
		{
			math::solvers::Solver::setup();
			math::solvers::Solver::m_watch_dof = m_analysis->m_model->m_mesh->m_nodes[m_watch_dof.m_node]->dof_index(m_watch_dof.m_dof);
		}
		void Solver::compute(void)
		{
			return;
		}
		void Solver::predictor(void)
		{
			return;
		}
		void Solver::corrector(void)
		{
			return;
		}

		//static
		Analysis* Solver::m_analysis = nullptr;
	}
}
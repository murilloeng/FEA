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
		Solver::Solver(void)
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

		//static
		Analysis* Solver::m_analysis = nullptr;
	}
}
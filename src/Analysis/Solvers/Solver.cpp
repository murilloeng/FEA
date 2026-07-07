//std
#include <cstring>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
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
			allocate();
			math::solvers::Solver::setup();
			math::solvers::Solver::m_watch_dof = m_analysis->m_model->m_mesh->m_nodes[m_watch_dof.m_node]->dof_index(m_watch_dof.m_dof);
		}
		void Solver::allocate(void)
		{
			//data
			math::solvers::Solver::allocate(m_analysis->m_assembler->m_dof_unknow);
			//setup
			memset(m_x_old, 0, m_size * sizeof(double));
		}

		//static
		Analysis* Solver::m_analysis = nullptr;
	}
}
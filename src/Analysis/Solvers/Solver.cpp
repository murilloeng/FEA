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

		//data
		WatchDOF& Solver::watch_dof(void)
		{
			return m_watch_dof;
		}

		uint32_t Solver::load_combination(void) const
		{
			return m_load_combination;
		}
		uint32_t Solver::load_combination(uint32_t load_combination)
		{
			return m_load_combination = load_combination;
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
			math::solvers::Solver::m_watch_dof = m_analysis->model()->m_mesh->node(m_watch_dof.m_node)->dof_index(m_watch_dof.m_dof);
		}
		void Solver::allocate(void)
		{
			//data
			math::solvers::Solver::allocate(m_analysis->m_assembler->dof_unknow());
			//setup
			memset(m_x_old, 0, m_size * sizeof(double));
		}

		//static
		Analysis* Solver::m_analysis = nullptr;
	}
}
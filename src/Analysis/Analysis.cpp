//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDOF.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Analysis::Analysis(void) : m_solver{nullptr}, m_assembler{new Assembler}
		{
			Solver::m_analysis = this;
			WatchDOF::m_analysis = this;
			Assembler::m_analysis = this;
		}
		
		//destructor
		Analysis::~Analysis(void)
		{
			delete m_solver;
			delete m_assembler;
		}

		//analysis
		void Analysis::setup(void)
		{
			m_solver->setup();
		}
		void Analysis::dof_apply(void)
		{
			m_model->m_mesh->m_nodes[m_solver->m_watch_dof.m_node]->m_dof_set |= 1 << uint32_t(m_solver->m_watch_dof.m_dof);
		}

		//static
		Model* Analysis::m_model = nullptr;
	}
}
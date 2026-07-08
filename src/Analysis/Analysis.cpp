//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDOF.hpp"
#include "FEA/inc/Analysis/Solvers/StaticLinear.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Analysis::Analysis(void) : m_solver{new StaticLinear}, m_assembler{new Assembler}
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
		void Analysis::check(void)
		{
			m_solver->check();
		}
		void Analysis::setup(void)
		{
			m_assembler->setup();
			m_solver->setup();
		}
		void Analysis::dof_apply(void)
		{
			m_model->m_mesh->m_nodes[m_solver->m_watch_dof.m_node]->m_dof_set |= 1 << uint32_t(m_solver->m_watch_dof.m_dof);
		}

		//data
		void Analysis::create_solver(Type type)
		{
			std::function<void(Solver*&)> fabric[] = {
				[](Solver*& solver){ solver = new StaticLinear; },
				[](Solver*& solver){ solver = new StaticNonlinear; }
			};
			delete m_solver;
			fabric[uint32_t(type)](m_solver);
		}

		//static
		Model* Analysis::m_model = nullptr;
	}
}
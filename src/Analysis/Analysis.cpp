//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/Type.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDOF.hpp"
#include "FEA/inc/Analysis/Solvers/StaticLinear.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Analysis::Analysis(void) : 
			m_type{Type::StaticLinear},
			m_assembler{new Assembler},
			m_solver_static_linear{new StaticLinear}, 
			m_solver_static_nonlinear{new StaticNonlinear}
		{
			Solver::m_analysis = this;
			WatchDOF::m_analysis = this;
			Assembler::m_analysis = this;
		}
	
		//destructor
		Analysis::~Analysis(void)
		{
			delete m_assembler;
			delete m_solver_static_linear;
			delete m_solver_static_nonlinear;
		}

		//serialization
		void Analysis::save(FILE* file) const
		{
			return;
		}

		//data
		Type Analysis::type(Type type)
		{
			return m_type = type;
		}
		Type Analysis::type(void) const
		{
			return m_type;
		}

		Model* Analysis::model(void)
		{
			return m_model;
		}

		Solver* Analysis::solver(void) const
		{
			return
				m_type == Type::StaticLinear ? (Solver*) m_solver_static_linear :
				m_type == Type::StaticNonlinear ? (Solver*) m_solver_static_nonlinear : nullptr; 
		}
		Assembler* Analysis::assembler(void) const
		{
			return m_assembler;
		}

		StaticLinear* Analysis::solver_static_linear(void) const
		{
			return m_solver_static_linear;
		}
		StaticNonlinear* Analysis::solver_static_nonlinear(void) const
		{
			return m_solver_static_nonlinear;
		}

		//analysis
		void Analysis::check(void)
		{
			solver()->check();
		}
		void Analysis::setup(void)
		{
			m_assembler->setup();
			solver()->setup();
		}
		void Analysis::dof_apply(void)
		{
			m_model->mesh()->node(solver()->m_watch_dof.m_node)->m_dof_set |= 1 << uint32_t(solver()->m_watch_dof.m_dof);
		}

		//static
		Model* Analysis::m_model = nullptr;
	}
}
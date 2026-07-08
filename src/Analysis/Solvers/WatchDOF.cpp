//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDOF.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		WatchDOF::WatchDOF(void) : m_node{0}, m_dof{mesh::nodes::DOF::Translation_1}
		{
			return;
		}

		//destructor
		WatchDOF::~WatchDOF(void)
		{
			return;
		}

		//analysis
		void WatchDOF::check(void)
		{
			if(m_node >= m_analysis->model()->m_mesh->nodes().size())
			{
				throw std::runtime_error("Error: Watch DOF has out of range node!");
			}
		}

		//data
		double WatchDOF::state(void) const
		{
			return m_analysis->model()->m_mesh->node(m_node)->state(m_dof);
		}
		double WatchDOF::velocity(void) const
		{
			return m_analysis->model()->m_mesh->node(m_node)->velocity(m_dof);
		}
		double WatchDOF::acceleration(void) const
		{
			return m_analysis->model()->m_mesh->node(m_node)->acceleration(m_dof);
		}

		//static
		Analysis* WatchDOF::m_analysis = nullptr;
	}
}
//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"

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
			if(m_node >= m_analysis->m_model->m_mesh->m_nodes.size())
			{
				throw std::runtime_error("Error: Watch DOF has out of range node!");
			}
		}

		//static
		Analysis* WatchDOF::m_analysis = nullptr;
	}
}
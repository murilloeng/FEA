//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
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
	}
}
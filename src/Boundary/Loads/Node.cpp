//FEA
#include "FEA/inc/Boundary/Loads/Node.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructor
			Node::Node(void) : m_node{0}, m_dof{mesh::nodes::DOF::Translation_1}
			{
				return;
			}
			
			//destructor
			Node::~Node(void)
			{
				return;
			}
		}
	}
}
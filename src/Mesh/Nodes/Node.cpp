//FEA
#include "FEA/inc/Mesh/Nodes/Node.hpp"

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			//constructor
			Node::Node(void) : m_dof{0}, m_position{0, 0, 0}
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
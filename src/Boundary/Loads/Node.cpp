//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
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

			//analysis
			void Node::check(void)
			{
				if(m_node >= m_boundary->m_model->m_mesh->m_nodes.size())
				{
					throw std::runtime_error("Error: Node load has out of range node!");
				}
			}
			void Node::setup(void)
			{
				m_dof_index = m_boundary->m_model->m_mesh->m_nodes[m_node]->dof_index(m_dof);
			}
		}
	}
}
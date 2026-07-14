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
			Node::Node(uint32_t node, mesh::nodes::DOF dof, double value) : m_node{node}, m_dof{dof}
			{
				m_value = value;
			}

			//destructor
			Node::~Node(void)
			{
				return;
			}

			//serialization
			void Node::save(FILE* file) const
			{
				Load::save(file);
				fprintf(file, "Node: %d DOF: %d ", m_node, uint32_t(m_dof));
			}

			//data
			uint32_t Node::node(void) const
			{
				return m_node;
			}
			uint32_t Node::node(uint32_t node)
			{
				return m_node = node;
			}

			uint32_t Node::dof_index(void) const
			{
				return m_dof_index;
			}

			mesh::nodes::DOF Node::dof(void) const
			{
				return m_dof;
			}
			mesh::nodes::DOF Node::dof(mesh::nodes::DOF dof)
			{
				return m_dof = dof;
			}

			//analysis
			void Node::check(void)
			{
				if(m_node >= m_boundary->model()->mesh()->nodes().size())
				{
					throw std::runtime_error("Error: Node load has out of range node!");
				}
			}
			void Node::setup(void)
			{
				m_dof_index = m_boundary->model()->mesh()->node(m_node)->dof_index(m_dof);
			}
		}
	}
}
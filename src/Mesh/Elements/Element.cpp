//std
#include <stdexcept>

//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

//Math
#include "Math/inc/Miscellaneous/bits.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Element::Element(void)
			{
				return;
			}

			//destructor
			Element::~Element(void)
			{
				return;
			}

			//serialization
			void Element::save(FILE* file) const
			{
				fprintf(file, "Index: %4d Nodes: %2zd - ", m_index, m_nodes.size());
				for(uint32_t node : m_nodes) fprintf(file, "%4d ", node);
			}

			//data
			uint32_t Element::index(void) const
			{
				return m_index;
			}
			uint32_t Element::index_node(uint32_t index) const
			{
				return m_nodes[index];
			}

			nodes::Node* Element::node(uint32_t index) const
			{
				return m_mesh->node(m_nodes[index]);
			}

			//analysis
			void Element::check(void)
			{
				for(uint32_t node : m_nodes)
				{
					if(node >= m_mesh->nodes().size())
					{
						throw std::runtime_error("Error: Element's node index is out of range!");
					}
				}
			}
			void Element::setup(void)
			{
				m_dof_indexes.clear();
				for(uint32_t i = 0; i < m_nodes.size(); i++)
				{
					const uint32_t dof_set = this->dof_set(i);
					for(uint32_t dof = 0; dof < uint32_t(nodes::DOF::Last); dof++)
					{
						if(dof_set & 1 << dof)
						{
							m_dof_indexes.push_back(node(i)->dof_index(nodes::DOF(dof)));
						}
					}
				}
			}

			//static
			Mesh* Element::m_mesh = nullptr;
		}
	}
}
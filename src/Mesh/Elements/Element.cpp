//std
#include <stdexcept>

//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

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

			//analysis
			void Element::check(void)
			{
				for(uint32_t node : m_nodes)
				{
					if(node >= m_mesh->m_nodes.size())
					{
						throw std::runtime_error("Error: Element's node index is out of range!");
					}
				}
			}
			void Element::setup(void)
			{
				return;
			}

			//data
			nodes::Node* Element::node(uint32_t index) const
			{
				return m_mesh->m_nodes[m_nodes[index]];
			}

			//static
			Mesh* Element::m_mesh = nullptr;
		}
	}
}
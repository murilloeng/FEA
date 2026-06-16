//FEA
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

			//data
			uint32_t Element::node(uint32_t index) const
			{
				return m_nodes[index];
			}
			uint32_t Element::node(uint32_t index, uint32_t node)
			{
				return m_nodes[index] = node;
			}
			const std::vector<uint32_t>& Element::nodes(void) const
			{
				return m_nodes;
			}
		}
	}
}
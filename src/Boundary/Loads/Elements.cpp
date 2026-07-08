//FEA
#include "FEA/inc/Boundary/Loads/Element.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
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
			uint32_t Element::element(uint32_t index) const
			{
				return m_elements[index];
			}
			const std::vector<uint32_t>& Element::elements(void) const
			{
				return m_elements;
			}
		}
	}
}
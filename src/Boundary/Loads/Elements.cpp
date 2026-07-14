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

			//serialization
			void Element::save(FILE* file) const
			{
				Load::save(file);
				fprintf(file, "Elements: %zd - ", m_elements.size());
				for(uint32_t element : m_elements) fprintf(file, "%4d ", element);
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
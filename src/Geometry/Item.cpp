//FEA
#include "FEA/inc/Geometry/Item.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Item::Item(uint32_t index, bool inverted) : m_inverted{inverted}, m_index{index}
		{
			return;
		}
		
		//destructor
		Item::~Item(void)
		{
			return;
		}

		//serialization
		void Item::save(FILE* file) const
		{
			fprintf(file, "%d %d ", m_index, m_inverted);
		}

		//data
		bool Item::inverted(void) const
		{
			return m_inverted;
		}
		bool Item::inverted(bool inverted)
		{
			return m_inverted = m_inverted;
		}

		uint32_t Item::index(void) const
		{
			return m_index;
		}
		uint32_t Item::index(uint32_t index)
		{
			return m_index = index;
		}
	}
}
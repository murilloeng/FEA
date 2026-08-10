//FEA
#include "FEA/inc/Geometry/Loop.hpp"

namespace fea
{
	namespace geometry
	{
		//constructor
		Loop::Loop(void)
		{
			return;
		}

		//destructor
		Loop::~Loop(void)
		{
			return;
		}

		//serialization
		void Loop::save(FILE* file) const
		{
			for(const Item& item : m_items) item.save(file);
		}

		//data
		const std::vector<Item>& Loop::items(void) const
		{
			return m_items;
		}
		void Loop::create_item(uint32_t index, bool inverted)
		{
			m_items.push_back(Item(index, inverted));
		}
	}
}
#pragma once

//std
#include <vector>
#include <cstdio>

//FEA
#include "FEA/inc/Geometry/Item.hpp"

namespace fea
{
	namespace geometry
	{
		class Geometry;
	}
}

namespace fea
{
	namespace geometry
	{
		class Loop
		{
		public:
			//constructor
			Loop(void);

			//destructor
			~Loop(void);

			//serialization
			void save(FILE*) const;

			//data
			void create_item(uint32_t, bool);
			const std::vector<Item>& items(void) const;

		private:
			//data
			std::vector<Item> m_items;

			//friends
			friend class fea::geometry::Geometry;
		};
	}
}
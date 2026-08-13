#pragma once

//std
#include <cstdio>
#include <cstdint>

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
		class Item
		{
		public:
			//constructor
			Item(uint32_t, bool);

			//destructor
			~Item(void);

			//serialization
			void save(FILE*) const;

			//data
			bool inverted(bool);
			bool inverted(void) const;

			uint32_t index(uint32_t);
			uint32_t index(void) const;

		private:
			//data
			bool m_inverted;
			uint32_t m_index;

			//friends
			friend class fea::geometry::Geometry;
		};
	}
}
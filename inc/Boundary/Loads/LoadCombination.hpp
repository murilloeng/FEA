#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	namespace boundary
	{
		class Boundary;
	}
}

namespace fea
{
	namespace boundary
	{
		class LoadCombination
		{
		public:
			//constructor
			LoadCombination(void);

			//destructor
			~LoadCombination(void);

			//analysis
			void check(void);
			void setup(void);

			//structs
			struct Item
			{
				double m_value = 1;
				bool m_fixed = false;
				uint32_t m_load_case = 0;
			};

			//data
			std::vector<Item> m_items;
			static Boundary* m_boundary;
		};
	}
}
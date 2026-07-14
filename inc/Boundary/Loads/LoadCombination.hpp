#pragma once

//std
#include <vector>
#include <cstdint>

//FEA
#include "FEA/inc/Boundary/Loads/LoadItem.hpp"

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
		private:
			//constructor
			LoadCombination(void);

			//destructor
			~LoadCombination(void);

			//serialization
			void save(FILE*) const;

		public:
			//create
			void create_load_item(uint32_t, double, bool);

			//data
			static Boundary* boundary(void);

			const LoadItem* load_item(uint32_t) const;
			const std::vector<LoadItem*>& load_items(void) const;

		private:
			//analysis
			void check(void);
			void setup(void);

			//data
			static Boundary* m_boundary;
			std::vector<LoadItem*> m_load_items;

			//friends
			friend class fea::boundary::Boundary;
		};
	}
}
#pragma once

//std
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class LoadCombination;
		}
	}
}

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Item
			{
			private:
				//constructors
				Item(LoadCombination*);

				//destructor
				~Item(void);

				//serialization
				void load(FILE*);
				void save(FILE*) const;

			public:
				//data
				bool fixed(bool);
				bool fixed(void) const;

				double value(double);
				double value(void) const;

				uint32_t load_case(uint32_t);
				uint32_t load_case(void) const;

				uint32_t index(void) const;
				LoadCombination* combination(void) const;

			private:
				//check
				bool check(void) const;

				//data
				bool m_fixed;
				double m_value;
				uint32_t m_index;
				uint32_t m_load_case;
				LoadCombination* const m_combination;

				//friends
				friend class LoadCombination;
			};
		}
	}
}
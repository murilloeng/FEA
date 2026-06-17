#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace boundary
	{
		class Boundary;
		namespace loads
		{
			class Item;
		}
	}
	namespace analysis
	{
		class Assembler;
	}
}

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class LoadCombination
			{
			private:
				//constructors
				LoadCombination(void);

				//destructor
				~LoadCombination(void);

				//serialization
				void load(FILE*);
				void save(FILE*) const;

			public:
				//data
				Item* item(uint32_t);

				const char* label(void) const;
				const char* label(const char*);

				static Boundary* boundary(void);

				const std::vector<Item*>& items(void) const;

				//list
				void remove_item(uint32_t);
				Item* create_item(bool, double, uint32_t);

				//index
				uint32_t index(void) const;

			private:
				//check
				bool check(void) const;

				//data
				uint32_t m_index;
				char m_label[200];
				std::vector<Item*> m_items;
				static Boundary* m_boundary;

				//friends
				friend class boundary::Boundary;
				friend class analysis::Assembler;
			};
		}
	}
}
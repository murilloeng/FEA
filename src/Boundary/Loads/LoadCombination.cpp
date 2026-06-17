//std
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stdexcept>

//FEA
#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/Item.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			LoadCombination::LoadCombination(void) : m_label("Combination")
			{
				return;
			}

			//destructor
			LoadCombination::~LoadCombination(void)
			{
				for(const Item* item : m_items) delete item;
			}

			//serialization
			void LoadCombination::load(FILE* file)
			{
				//size
				uint32_t ni;
				if(fscanf(file, "%s %d", m_label, &ni) != 2)
				{
					throw std::runtime_error("Load combination loading error!");
				}
				//cases
				m_items.resize(ni);
				for(uint32_t i = 0; i < ni; i++)
				{
					m_items[i] = new Item(this);
					m_items[i]->m_index = i;
					m_items[i]->load(file);
				}
			}
			void LoadCombination::save(FILE* file) const
			{
				//size
				fprintf(file, "%s %zd ", m_label, m_items.size());
				//cases
				for(const Item* item : m_items)
				{
					item->save(file);
				}
			}

			//data
			Item* LoadCombination::item(uint32_t index)
			{
				if(index >= m_items.size())
				{
					throw std::runtime_error("Load combination's item index is out of range!");
				}
				return m_items[index];
			}

			const char* LoadCombination::label(void) const
			{
				return m_label;
			}
			const char* LoadCombination::label(const char* label)
			{
				return strcpy(m_label, label);
			}

			Boundary* LoadCombination::boundary(void)
			{
				return m_boundary;
			}

			const std::vector<Item*>& LoadCombination::items(void) const
			{
				return m_items;
			}

			//list
			void LoadCombination::remove_item(uint32_t index)
			{
				//check
				if(index >= m_items.size())
				{
					throw std::runtime_error("Load combination's item index is out of range!");
				}
				//remove
				m_items.erase(m_items.begin() + index);
				for(uint32_t i = index; i < m_items.size(); i++)
				{
					m_items[i]->m_index--;
				}
			}
			Item* LoadCombination::create_item(bool fixed, double value, uint32_t load_case)
			{
				//data
				Item* item = new Item(this);
				//setup
				item->m_fixed = fixed;
				item->m_value = value;
				item->m_load_case = load_case;
				item->m_index = m_items.size();
				//list
				m_items.push_back(item);
				//return
				return item;
			}

			//index
			uint32_t LoadCombination::index(void) const
			{
				return m_index;
			}

			//check
			bool LoadCombination::check(void) const
			{
				for(const Item* item : m_items)
				{
					if(!item->check()) return false;
				}
				return true;
			}

			//static data
			Boundary* LoadCombination::m_boundary = nullptr;
		}
	}
}
//std
#include <cstdlib>
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
			Item::Item(LoadCombination* combination) : 
				m_fixed(false), m_value(1), m_index(0), m_load_case(0), m_combination(combination)
			{
				return;
			}

			//destructor
			Item::~Item(void)
			{
				return;
			}

			//serialization
			void Item::load(FILE* file)
			{
				uint32_t fixed;
				if(fscanf(file, "%d %lf %d", &fixed, &m_value, &m_load_case) != 3)
				{
					throw std::runtime_error("Item loading error!");
				}
				m_fixed = fixed;
			}
			void Item::save(FILE* file) const
			{
				fprintf(file, "%01d %+.6e %02d ", m_fixed, m_value, m_load_case);
			}

			//data
			bool Item::fixed(void) const
			{
				return m_fixed;
			}
			bool Item::fixed(bool fixed)
			{
				return m_fixed = fixed;
			}

			double Item::value(void) const
			{
				return m_value;
			}
			double Item::value(double value)
			{
				return m_value = value;
			}

			uint32_t Item::load_case(void) const
			{
				return m_load_case;
			}
			uint32_t Item::load_case(uint32_t load_case)
			{
				return m_load_case = load_case;
			}

			uint32_t Item::index(void) const
			{
				return m_index;
			}
			LoadCombination* Item::combination(void) const
			{
				return m_combination;
			}

			//check
			bool Item::check(void) const
			{
				if(m_load_case >= m_combination->boundary()->load_cases().size())
				{
					printf("Item %d of combination %d has a out of range load case's index!\n", m_index, m_combination->index());
					return false;
				}
				return true;
			}
		}
	}
}
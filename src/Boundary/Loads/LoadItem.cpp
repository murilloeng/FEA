//FEA
#include "FEA/inc/Boundary/Loads/LoadItem.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		LoadItem::LoadItem(uint32_t load_case, double value, bool fixed) : m_fixed{fixed}, m_value{value}, m_load_case{load_case}
		{
			return;
		}
		
		//destructor
		LoadItem::~LoadItem(void)
		{
			return;
		}

		//data
		bool LoadItem::fixed(void) const
		{
			return m_fixed;
		}
		bool LoadItem::fixed(bool fixed)
		{
			return m_fixed = fixed;
		}

		double LoadItem::value(void) const
		{
			return m_value;
		}
		double LoadItem::value(double value)
		{
			return m_value = value;
		}

		uint32_t LoadItem::load_case(void) const
		{
			return m_load_case;
		}
		uint32_t LoadItem::load_case(uint32_t laod_case)
		{
			return m_load_case = laod_case;
		}
	}
}
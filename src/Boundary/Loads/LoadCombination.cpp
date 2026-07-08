//FEA
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		LoadCombination::LoadCombination(void)
		{
			return;
		}
	
		//destructor
		LoadCombination::~LoadCombination(void)
		{
			for(const LoadItem* load_item : m_load_items) delete load_item;
		}

		//create
		void LoadCombination::create_load_item(uint32_t load_case, double value, bool fixed)
		{
			m_load_items.push_back(new LoadItem{load_case, value, fixed});
		}

		//data
		Boundary* LoadCombination::boundary(void)
		{
			return m_boundary;
		}

		const LoadItem* LoadCombination::load_item(uint32_t index) const
		{
			return m_load_items[index];
		}
		const std::vector<LoadItem*>& LoadCombination::load_items(void) const
		{
			return m_load_items;
		}

		//analysis
		void LoadCombination::check(void)
		{
			return;
		}
		void LoadCombination::setup(void)
		{
			return;
		}

		//static
		Boundary* LoadCombination::m_boundary = nullptr;
	}
}
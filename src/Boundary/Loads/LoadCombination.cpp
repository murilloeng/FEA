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

		//serialization
		void LoadCombination::save(FILE* file) const
		{
			//header
			fprintf(file, "\n");
			fprintf(file, "### Load Combination %d:\n", m_index);
			//load Items
			fprintf(file, "\n");
			fprintf(file, "#### Load items: %zd\n", m_load_items.size());
			for(const LoadItem* load_item : m_load_items) load_item->save(file), fprintf(file, "\n");
		}

		//create
		void LoadCombination::create_load_item(uint32_t load_case, double value, bool fixed)
		{
			//data
			const uint32_t ni = m_load_items.size();
			LoadItem* load_item = new LoadItem{load_case, value, fixed};
			//append
			load_item->m_index = ni;
			m_load_items.push_back(load_item);
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
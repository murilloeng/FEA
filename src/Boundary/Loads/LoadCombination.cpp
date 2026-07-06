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
			return;
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
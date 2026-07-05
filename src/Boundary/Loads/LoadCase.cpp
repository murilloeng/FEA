//FEA
#include "FEA/inc/Boundary/Loads/Load.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		LoadCase::LoadCase(void)
		{
			return;
		}
		
		//destructor
		LoadCase::~LoadCase(void)
		{
			for(const loads::Load* load : m_loads) delete load;
		}

		//static
		Boundary* LoadCase::m_boundary = nullptr;
	}
}
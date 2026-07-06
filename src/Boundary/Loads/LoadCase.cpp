//FEA
#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/Element.hpp"
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
			for(const loads::Node* load : m_loads_nodes) delete load;
			for(const loads::Element* load : m_loads_elements) delete load;
		}

		//analysis
		void LoadCase::check(void)
		{
			return;
		}
		void LoadCase::setup(void)
		{
			return;
		}

		//static
		Boundary* LoadCase::m_boundary = nullptr;
	}
}
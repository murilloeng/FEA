//FEA
#include "FEA/inc/Boundary/Boundary.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Boundary::Boundary(void)
		{
			return;
		}
		
		//destructor
		Boundary::~Boundary(void)
		{
			return;
		}

		//static
		Model* Boundary::m_model = nullptr;
	}
}
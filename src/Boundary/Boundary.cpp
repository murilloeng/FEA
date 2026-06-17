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

		//data
		Model* Boundary::model(void)
		{
			return m_model;
		}

		//static data
		Model* Boundary::m_model = nullptr;
	}
}
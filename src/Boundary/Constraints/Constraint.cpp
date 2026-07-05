//FEA
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Constraint::Constraint(void)
		{
			return;
		}
		
		//destructor
		Constraint::~Constraint(void)
		{
			return;
		}

		//static
		Boundary* Constraint::m_boundary = nullptr;
	}
}
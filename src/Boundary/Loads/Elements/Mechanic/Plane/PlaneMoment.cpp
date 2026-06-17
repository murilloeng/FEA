//std
#include <cmath>

//FEA
#include "FEA/inc/Boundary/Loads/Types.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Plane/PlaneMoment.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			PlaneMoment::PlaneMoment(LoadCase* load_case) : Plane(load_case)
			{
				return;
			}

			//destructor
			PlaneMoment::~PlaneMoment(void)
			{
				return;
			}

			//types
			Type PlaneMoment::type(void) const
			{
				return Type::PlaneMoment;
			}
		}
	}
}
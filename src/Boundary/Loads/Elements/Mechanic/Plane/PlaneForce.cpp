//std
#include <cmath>

//FEA
#include "FEA/inc/Boundary/Loads/Types.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Plane/PlaneForce.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			PlaneForce::PlaneForce(LoadCase* load_case) : Plane(load_case)
			{
				return;
			}

			//destructor
			PlaneForce::~PlaneForce(void)
			{
				return;
			}

			//type
			Type PlaneForce::type(void) const
			{
				return Type::PlaneForce;
			}
		}
	}
}
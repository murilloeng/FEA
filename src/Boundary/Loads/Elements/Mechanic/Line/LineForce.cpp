//std
#include <cmath>

//FEA
#include "FEA/inc/Boundary/Loads/Types.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Line/LineForce.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			LineForce::LineForce(LoadCase* load_case) : Line(load_case)
			{
				return;
			}

			//destructor
			LineForce::~LineForce(void)
			{
				return;
			}

			//types
			Type LineForce::type(void) const
			{
				return Type::LineForce;
			}
		}
	}
}
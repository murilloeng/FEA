//std
#include <cmath>

//FEA
#include "FEA/inc/Boundary/Loads/Types.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Line/LineMoment.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			LineMoment::LineMoment(LoadCase* load_case) : Line(load_case)
			{
				return;
			}

			//destructor
			LineMoment::~LineMoment(void)
			{
				return;
			}

			//types
			Type LineMoment::type(void) const
			{
				return Type::LineMoment;
			}
		}
	}
}
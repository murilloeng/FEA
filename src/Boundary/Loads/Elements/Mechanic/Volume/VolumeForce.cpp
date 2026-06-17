//FEA
#include "FEA/inc/Boundary/Loads/Types.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Volume/VolumeForce.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			VolumeForce::VolumeForce(LoadCase* load_case) : Mechanic(load_case)
			{
				return;
			}

			//destructor
			VolumeForce::~VolumeForce(void)
			{
				return;
			}

			//types
			Type VolumeForce::type(void) const
			{
				return Type::VolumeForce;
			}
		}
	}
}
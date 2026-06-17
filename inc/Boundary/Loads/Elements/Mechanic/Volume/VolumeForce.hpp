#pragma once

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Mechanic.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class VolumeForce : public Mechanic
			{
			private:
				//constructors
				VolumeForce(LoadCase*);

				//destructor
				~VolumeForce(void) override;

			public:
				//type
				Type type(void) const override;

				//friends
				friend class Element;
			};
		}
	}
}
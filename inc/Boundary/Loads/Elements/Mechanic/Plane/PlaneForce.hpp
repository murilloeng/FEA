#pragma once

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Plane/Plane.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class PlaneForce : public Plane
			{
			private:
				//constructors
				PlaneForce(LoadCase*);

				//destructor
				~PlaneForce(void) override;

			public:
				//type
				Type type(void) const override;

				//friends
				friend class Element;
			};
		}
	}
}
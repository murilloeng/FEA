#pragma once

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Plane/Plane.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class PlaneMoment : public Plane
			{
			protected:
				//constructors
				PlaneMoment(LoadCase*);

				//destructor
				~PlaneMoment(void) override;

			public:
				//type
				Type type(void) const override;

				//friends
				friend class Element;
			};
		}
	}
}
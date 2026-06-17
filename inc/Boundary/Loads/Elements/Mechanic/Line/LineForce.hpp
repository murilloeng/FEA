#pragma once

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Line/Line.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class LineForce : public Line
			{
			private:
				//constructors
				LineForce(LoadCase*);

				//destructor
				~LineForce(void) override;

			public:
				//type
				Type type(void) const override;

				//friends
				friend class Element;
			};
		}
	}
}
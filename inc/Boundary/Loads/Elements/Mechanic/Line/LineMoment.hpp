#pragma once

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Line/Line.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class LineMoment : public Line
			{
			private:
				//constructors
				LineMoment(LoadCase*);

				//destructor
				~LineMoment(void) override;

			public:
				//type
				Type type(void) const override;

				//friends
				friend class Element;
			};
		}
	}
}
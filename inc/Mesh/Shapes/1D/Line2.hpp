#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Line2 : public Line
			{
			public:
				//constructor
				Line2(void);

				//destructor
				virtual ~Line2(void);

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Line3 : public Line
			{
			public:
				//constructor
				Line3(void);

				//destructor
				virtual ~Line3(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
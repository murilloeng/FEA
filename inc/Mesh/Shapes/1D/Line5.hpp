#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Line5 : public Line
			{
			public:
				//constructor
				Line5(void);

				//destructor
				virtual ~Line5(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Triangle.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Triangle9 : public Triangle
			{
			public:
				//constructor
				Triangle9(void);

				//destructor
				~Triangle9(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
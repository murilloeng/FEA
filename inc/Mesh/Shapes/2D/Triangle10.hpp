#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Triangle.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Triangle10 : public Triangle
			{
			public:
				//constructor
				Triangle10(void);

				//destructor
				~Triangle10(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
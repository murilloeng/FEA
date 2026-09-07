#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Triangle.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Triangle6 : public Triangle
			{
			public:
				//constructor
				Triangle6(void);

				//destructor
				~Triangle6(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
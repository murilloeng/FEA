#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Triangle.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tri6 : public Triangle
			{
			public:
				//constructor
				Tri6(void);

				//destructor
				~Tri6(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
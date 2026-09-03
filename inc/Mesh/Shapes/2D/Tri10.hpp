#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Triangle.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tri10 : public Triangle
			{
			public:
				//constructor
				Tri10(void);

				//destructor
				~Tri10(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
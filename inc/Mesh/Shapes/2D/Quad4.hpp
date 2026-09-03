#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quadrangle.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Quad4 : public Quadrangle
			{
			public:
				//constructor
				Quad4(void);

				//destructor
				~Quad4(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quadrangle.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Quad8 : public Quadrangle
			{
			public:
				//constructor
				Quad8(void);

				//destructor
				~Quad8(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
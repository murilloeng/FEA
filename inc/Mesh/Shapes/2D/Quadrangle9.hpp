#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quadrangle.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Quadrangle9 : public Quadrangle
			{
			public:
				//constructor
				Quadrangle9(void);

				//destructor
				~Quadrangle9(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
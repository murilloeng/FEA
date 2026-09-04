#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Quad9 : public Quad
			{
			public:
				//constructor
				Quad9(void);

				//destructor
				~Quad9(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
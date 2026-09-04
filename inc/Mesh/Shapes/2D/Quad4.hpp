#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Quad4 : public Quad
			{
			public:
				//constructor
				Quad4(void);

				//destructor
				~Quad4(void);

				//geometry
				uint32_t vertices(void) const override;
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
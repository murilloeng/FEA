#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tri12 : public Tri
			{
			public:
				//constructor
				Tri12(void);

				//destructor
				~Tri12(void);

				//geometry
				uint32_t vertices(void) const override;
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
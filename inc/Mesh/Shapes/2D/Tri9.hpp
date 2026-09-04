#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tri9 : public Tri
			{
			public:
				//constructor
				Tri9(void);

				//destructor
				~Tri9(void);

				//geometry
				uint32_t vertices(void) const override;
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
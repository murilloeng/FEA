#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tri3 : public Tri
			{
			public:
				//constructor
				Tri3(void);

				//destructor
				~Tri3(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
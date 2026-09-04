#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tri15 : public Tri
			{
			public:
				//constructor
				Tri15(void);

				//destructor
				~Tri15(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
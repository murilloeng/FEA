#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Brick.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Brick27 : public Brick
			{
			public:
				//constructor
				Brick27(void);

				//destructor
				~Brick27(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
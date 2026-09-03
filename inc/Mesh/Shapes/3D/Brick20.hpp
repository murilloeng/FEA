#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Brick.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Brick20 : public Brick
			{
			public:
				//constructor
				Brick20(void);

				//destructor
				~Brick20(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
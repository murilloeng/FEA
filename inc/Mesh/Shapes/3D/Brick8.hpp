#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Brick.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Brick8 : public Brick
			{
			public:
				//constructor
				Brick8(void);

				//destructor
				~Brick8(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
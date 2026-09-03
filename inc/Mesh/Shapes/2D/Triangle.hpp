#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Plane.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Triangle : public Plane
			{
			protected:
				//constructor
				Triangle(uint32_t);

				//destructor
				~Triangle(void);

			public:
				//integration
				void point(double&, double*, uint32_t) const override;
			};
		}
	}
}
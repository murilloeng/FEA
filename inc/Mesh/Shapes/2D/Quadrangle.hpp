#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Plane.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Quadrangle : public Plane
			{
			protected:
				//constructor
				Quadrangle(uint32_t);

				//destructor
				virtual ~Quadrangle(void);

			public:
				//integration
				void point(double&, double*, uint32_t) const override;
			};
		}
	}
}
#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Plane.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tri : public Plane
			{
			protected:
				//constructor
				Tri(uint32_t);

				//destructor
				virtual ~Tri(void);

			public:
				//integration
				void point(double&, double*, uint32_t) const override;
			};
		}
	}
}
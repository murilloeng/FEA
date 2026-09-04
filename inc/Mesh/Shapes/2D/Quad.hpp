#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/2D/Plane.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Quad : public Plane
			{
			protected:
				//constructor
				Quad(uint32_t);

				//destructor
				virtual ~Quad(void);

			public:
				//geometry
				uint32_t edges(void) const override;

				//integration
				void point(double&, double*, uint32_t) const override;
			};
		}
	}
}
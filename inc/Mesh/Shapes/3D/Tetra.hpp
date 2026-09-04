#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Volume.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tetra : public Volume
			{
			protected:
				//constructor
				Tetra(uint32_t);

				//destructor
				virtual ~Tetra(void);

				//geometry
				uint32_t faces(void) const override;
				uint32_t edges(void) const override;

			public:
				//integration
				void point(double&, double*, uint32_t) const override;
			};
		}
	}
}
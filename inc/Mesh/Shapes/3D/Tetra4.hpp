#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Tetra.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tetra4 : public Tetra
			{
			public:
				//constructor
				Tetra4(void);

				//destructor
				~Tetra4(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
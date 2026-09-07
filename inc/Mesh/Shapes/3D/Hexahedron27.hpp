#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Hexahedron.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Hexahedron27 : public Hexahedron
			{
			public:
				//constructor
				Hexahedron27(void);

				//destructor
				~Hexahedron27(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
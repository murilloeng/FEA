#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Hexahedron.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Hexahedron8 : public Hexahedron
			{
			public:
				//constructor
				Hexahedron8(void);

				//destructor
				~Hexahedron8(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
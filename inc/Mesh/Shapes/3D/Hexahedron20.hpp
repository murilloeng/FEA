#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Hexahedron.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Hexahedron20 : public Hexahedron
			{
			public:
				//constructor
				Hexahedron20(void);

				//destructor
				~Hexahedron20(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
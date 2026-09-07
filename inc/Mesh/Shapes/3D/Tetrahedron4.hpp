#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Tetrahedron.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tetrahedron4 : public Tetrahedron
			{
			public:
				//constructor
				Tetrahedron4(void);

				//destructor
				~Tetrahedron4(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
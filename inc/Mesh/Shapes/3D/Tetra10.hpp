#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Tetra.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Tetra10 : public Tetra
			{
			public:
				//constructor
				Tetra10(void);

				//destructor
				~Tetra10(void);
				
				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
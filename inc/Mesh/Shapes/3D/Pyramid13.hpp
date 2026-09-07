#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Pyramid.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Pyramid13 : public Pyramid
			{
			public:
				//constructor
				Pyramid13(void);

				//destructor
				~Pyramid13(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
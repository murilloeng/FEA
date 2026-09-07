#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Prism.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Prism15 : public Prism
			{
			public:
				//constructor
				Prism15(void);

				//destructor
				~Prism15(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
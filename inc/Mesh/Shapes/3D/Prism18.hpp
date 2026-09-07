#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Prism.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Prism18 : public Prism
			{
			public:
				//constructor
				Prism18(void);

				//destructor
				~Prism18(void);

				//geometry
				uint32_t vertices(void) const override;

				//shape
				void function(double*, const double*) const override;
				void gradient(double*, const double*) const override;
			};
		}
	}
}
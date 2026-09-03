#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/3D/Volume.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Brick : public Volume
			{
			protected:
				//constructor
				Brick(uint32_t);

				//destructor
				virtual ~Brick(void);

			public:
				//integration
				void point(double&, double*, uint32_t) const override;
			};
		}
	}
}
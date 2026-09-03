#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/Shape.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Volume : public Shape
			{
			protected:
				//constructor
				Volume(void);

				//destructor
				virtual ~Volume(void);

			public:
				//shape
				uint32_t dimension(void) const override;

				//integration
				uint32_t points(void) const override;
			};
		}
	}
}
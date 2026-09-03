#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/Shape.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Line : public Shape
			{
			protected:
				//constructor
				Line(void);

				//destructor
				virtual ~Line(void);

			public:
				//shape
				uint32_t dimension(void) const override;

				//integration
				void point(double&, double*, uint32_t) const override;
			};
		}
	}
}
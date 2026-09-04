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
				Line(uint32_t);

				//destructor
				virtual ~Line(void);

			public:
				//geometry
				uint32_t edges(void) const override;
				uint32_t faces(void) const override;
				uint32_t dimension(void) const override;

				//integration
				uint32_t points(void) const;
				void point(double&, double*, uint32_t) const override;
			};
		}
	}
}
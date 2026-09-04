#pragma once

//FEA
#include "FEA/inc/Mesh/Shapes/Shape.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			class Plane : public Shape
			{
			protected:
				//constructor
				Plane(uint32_t);

				//destructor
				virtual ~Plane(void);

				//data
				double thickness(double);
				double thickness(void) const;

			public:
				//geometry
				uint32_t faces(void) const override;
				uint32_t dimension(void) const override;

				//integration
				uint32_t points(void) const override;

			protected:
				//data
				double m_thickness;
			};
		}
	}
}
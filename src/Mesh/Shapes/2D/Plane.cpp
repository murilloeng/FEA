//FEA
#include "FEA/inc/Mesh/Shapes/2D/Plane.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Plane::Plane(uint32_t order) : Shape(order), m_thickness{1}
			{
				return;
			}

			//destructor
			Plane::~Plane(void)
			{
				return;
			}

			//data
			double Plane::thickness(void) const
			{
				return m_thickness;
			}
			double Plane::thickness(double thickness)
			{
				return m_thickness = thickness;
			}

			//shape
			uint32_t Plane::dimension(void) const
			{
				return 2U;
			}

			//integration
			uint32_t Plane::points(void) const
			{
				const uint32_t order = m_quadrature.order();
				return order * order;
			}
		}
	}
}
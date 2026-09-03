//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line::Line(uint32_t order) : Shape(order)
			{
				return;
			}
			
			//destructor
			Line::~Line(void)
			{
				return;
			}

			//shape
			uint32_t Line::dimension(void) const
			{
				return 1U;
			}

			//integration
			uint32_t Line::points(void) const
			{
				return m_quadrature.order();
			}
			void Line::point(double& w, double* s, uint32_t index) const
			{
				w = m_quadrature.weight(index);
				s[0] = m_quadrature.point(index);
			}
		}
	}
}
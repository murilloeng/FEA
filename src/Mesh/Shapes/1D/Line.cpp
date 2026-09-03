//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line::Line(void) : Shape(2)
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
			void Line::point(double& w, double* s, uint32_t index) const
			{
				w = m_quadrature.weight(index);
				s[0] = m_quadrature.point(index);
			}
		}
	}
}
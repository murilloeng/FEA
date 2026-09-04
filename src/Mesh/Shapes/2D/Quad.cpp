//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Quad::Quad(uint32_t order) : Plane(order)
			{
				return;
			}

			//destructor
			Quad::~Quad(void)
			{
				return;
			}

			//integration
			void Quad::point(double& w, double* s, uint32_t index) const
			{
				//data
				const uint32_t i = index % m_quadrature.order();
				const uint32_t j = index / m_quadrature.order();
				//point
				s[0] = m_quadrature.point(i);
				s[1] = m_quadrature.point(j);
				w = m_quadrature.weight(i) * m_quadrature.weight(j);
			}
		}
	}
}
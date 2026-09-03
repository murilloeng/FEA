//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quadrangle.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Quadrangle::Quadrangle(uint32_t order) : Plane(order)
			{
				return;
			}

			//destructor
			Quadrangle::~Quadrangle(void)
			{
				return;
			}

			//integration
			void Quadrangle::point(double& w, double* s, uint32_t index) const
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
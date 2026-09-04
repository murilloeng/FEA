//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri::Tri(uint32_t order) : Plane(order)
			{
				return;
			}

			//destructor
			Tri::~Tri(void)
			{
				return;
			}

			//geometry
			uint32_t Tri::edges(void) const
			{
				return 3U;
			}

			//integration
			void Tri::point(double& w, double* p, uint32_t index) const
			{
				//data
				const uint32_t i = index / m_quadrature.order();
				const uint32_t j = index % m_quadrature.order();
				//point
				const double e = m_quadrature.point(i);
				const double n = m_quadrature.point(j);
				const double wi = m_quadrature.weight(i);
				const double wj = m_quadrature.weight(j);
				//return
				w = wi * wj * (1 + n) / 2;
				p[0] = (1 + e) * (1 + n) / 2 - 1;
				p[1] = (1 - e) * (1 + n) / 2 - 1;
			}
		}
	}
}
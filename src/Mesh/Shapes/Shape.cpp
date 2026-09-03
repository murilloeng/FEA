//FEA
#include "FEA/inc/Mesh/Shapes/Shape.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Shape::Shape(uint32_t order) : m_quadrature{order}
			{
				return;
			}

			//destructor
			Shape::~Shape(void)
			{
				return;
			}

			//inetgeration
			math::quadrature::Quadrature& Shape::quadrature(void)
			{
				return m_quadrature;
			}
		}
	}
}
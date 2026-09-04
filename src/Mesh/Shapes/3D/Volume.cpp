//FEA
#include "FEA/inc/Mesh/Shapes/3D/Volume.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Volume::Volume(uint32_t order) : Shape(order)
			{
				return;
			}

			//destructor
			Volume::~Volume(void)
			{
				return;
			}

			//shape
			uint32_t Volume::dimension(void) const
			{
				return 3U;
			}

			//integration
			uint32_t Volume::points(void) const
			{
				const uint32_t order = m_quadrature.order();
				return order * order * order;
			}
		}
	}
}
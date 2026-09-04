//FEA
#include "FEA/inc/Mesh/Shapes/3D/Brick.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Brick::Brick(uint32_t order) : Volume(order)
			{
				return;
			}

			//destructor
			Brick::~Brick(void)
			{
				return;
			}

			//geometry
			uint32_t Brick::edges(void) const
			{
				return 12U;
			}
			uint32_t Brick::faces(void) const
			{
				return 6U;
			}

			//integration
			void Brick::point(double& w, double* s, uint32_t index) const
			{
				return;
			}
		}
	}
}
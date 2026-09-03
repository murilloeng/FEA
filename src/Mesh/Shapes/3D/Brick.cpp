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

			//integration
			void Brick::point(double& w, double* s, uint32_t index) const
			{
				return;
			}
		}
	}
}
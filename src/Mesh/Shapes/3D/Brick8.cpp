//FEA
#include "FEA/inc/Mesh/Shapes/3D/Brick8.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Brick8::Brick8(void) : Brick(1)
			{
				return;
			}

			//destructor
			Brick8::~Brick8(void)
			{
				return;
			}

			//geometry
			uint32_t Brick8::vertices(void) const
			{
				return 8U;
			}

			//integration
			void Brick8::function(double* N, const double* s) const
			{
				return;
			}
			void Brick8::gradient(double* B, const double* s) const
			{
				return;
			}
		}
	}
}
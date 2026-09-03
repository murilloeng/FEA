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
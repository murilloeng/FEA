//FEA
#include "FEA/inc/Mesh/Shapes/3D/Brick20.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Brick20::Brick20(void) : Brick(1)
			{
				return;
			}

			//destructor
			Brick20::~Brick20(void)
			{
				return;
			}

			//integration
			void Brick20::function(double* N, const double* s) const
			{
				return;
			}
			void Brick20::gradient(double* B, const double* s) const
			{
				return;
			}
		}
	}
}
//FEA
#include "FEA/inc/Mesh/Shapes/3D/Brick27.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Brick27::Brick27(void) : Brick(1)
			{
				return;
			}

			//destructor
			Brick27::~Brick27(void)
			{
				return;
			}

			//integration
			void Brick27::function(double* N, const double* s) const
			{
				return;
			}
			void Brick27::gradient(double* B, const double* s) const
			{
				return;
			}
		}
	}
}
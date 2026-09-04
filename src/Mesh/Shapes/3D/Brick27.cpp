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

			//geometry
			uint32_t Brick27::vertices(void) const
			{
				return 27U;
			}

			//integration
			void Brick27::function(double* N, const double* p) const
			{
				return;
			}
			void Brick27::gradient(double* B, const double* p) const
			{
				return;
			}
		}
	}
}
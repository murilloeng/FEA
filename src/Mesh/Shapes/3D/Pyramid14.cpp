//FEA
#include "FEA/inc/Mesh/Shapes/3D/Pyramid14.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Pyramid14::Pyramid14(void) : Pyramid(1)
			{
				return;
			}

			//destructor
			Pyramid14::~Pyramid14(void)
			{
				return;
			}

			//geometry
			uint32_t Pyramid14::vertices(void) const
			{
				return 14U;
			}

			//integration
			void Pyramid14::function(double* N, const double* p) const
			{
				return;
			}
			void Pyramid14::gradient(double* B, const double* p) const
			{
				return;
			}
		}
	}
}
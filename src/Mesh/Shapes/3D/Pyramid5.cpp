//FEA
#include "FEA/inc/Mesh/Shapes/3D/Pyramid5.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Pyramid5::Pyramid5(void) : Pyramid(1)
			{
				return;
			}

			//destructor
			Pyramid5::~Pyramid5(void)
			{
				return;
			}

			//geometry
			uint32_t Pyramid5::vertices(void) const
			{
				return 5U;
			}

			//integration
			void Pyramid5::function(double* N, const double* p) const
			{
				return;
			}
			void Pyramid5::gradient(double* B, const double* p) const
			{
				return;
			}
		}
	}
}
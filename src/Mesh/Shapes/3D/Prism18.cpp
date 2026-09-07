//FEA
#include "FEA/inc/Mesh/Shapes/3D/Prism18.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Prism18::Prism18(void) : Prism(1)
			{
				return;
			}

			//destructor
			Prism18::~Prism18(void)
			{
				return;
			}

			//geometry
			uint32_t Prism18::vertices(void) const
			{
				return 18U;
			}

			//integration
			void Prism18::function(double* N, const double* p) const
			{
				return;
			}
			void Prism18::gradient(double* B, const double* p) const
			{
				return;
			}
		}
	}
}
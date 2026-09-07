//FEA
#include "FEA/inc/Mesh/Shapes/3D/Prism15.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Prism15::Prism15(void) : Prism(1)
			{
				return;
			}

			//destructor
			Prism15::~Prism15(void)
			{
				return;
			}

			//geometry
			uint32_t Prism15::vertices(void) const
			{
				return 15U;
			}

			//integration
			void Prism15::function(double* N, const double* p) const
			{
				return;
			}
			void Prism15::gradient(double* B, const double* p) const
			{
				return;
			}
		}
	}
}
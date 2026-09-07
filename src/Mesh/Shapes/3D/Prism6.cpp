//FEA
#include "FEA/inc/Mesh/Shapes/3D/Prism6.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Prism6::Prism6(void) : Prism(1)
			{
				return;
			}

			//destructor
			Prism6::~Prism6(void)
			{
				return;
			}

			//geometry
			uint32_t Prism6::vertices(void) const
			{
				return 6U;
			}

			//integration
			void Prism6::function(double* N, const double* p) const
			{
				return;
			}
			void Prism6::gradient(double* B, const double* p) const
			{
				return;
			}
		}
	}
}
//FEA
#include "FEA/inc/Mesh/Shapes/3D/Pyramid13.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Pyramid13::Pyramid13(void) : Pyramid(1)
			{
				return;
			}

			//destructor
			Pyramid13::~Pyramid13(void)
			{
				return;
			}

			//geometry
			uint32_t Pyramid13::vertices(void) const
			{
				return 13U;
			}

			//integration
			void Pyramid13::function(double* N, const double* p) const
			{
				return;
			}
			void Pyramid13::gradient(double* B, const double* p) const
			{
				return;
			}
		}
	}
}
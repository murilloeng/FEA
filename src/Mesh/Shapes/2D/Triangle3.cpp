//FEA
#include "FEA/inc/Mesh/Shapes/2D/Triangle3.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Triangle3::Triangle3(void) : Triangle(1)
			{
				return;
			}

			//destructor
			Triangle3::~Triangle3(void)
			{
				return;
			}

			//geometry
			uint32_t Triangle3::vertices(void) const
			{
				return 3U;
			}

			//integration
			void Triangle3::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[0] = -(r + s) / 2;
				N[1] = +(1 + r) / 2;
				N[2] = +(1 + s) / 2;
			}
			void Triangle3::gradient(double* B, const double* p) const
			{
				//gradient r
				B[2 + 0] = +0;
				B[0 + 0] = -1.0 / 2;
				B[1 + 0] = +1.0 / 2;
				//gradient s
				B[1 + 3] = +0;
				B[0 + 3] = -1.0 / 2;
				B[2 + 3] = +1.0 / 2;
			}
		}
	}
}
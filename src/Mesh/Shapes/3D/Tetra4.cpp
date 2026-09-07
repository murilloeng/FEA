//FEA
#include "FEA/inc/Mesh/Shapes/3D/Tetra4.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tetra4::Tetra4(void) : Tetra(1)
			{
				return;
			}

			//destructor
			Tetra4::~Tetra4(void)
			{
				return;
			}

			//geometry
			uint32_t Tetra4::vertices(void) const
			{
				return 4U;
			}

			//integration
			void Tetra4::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				//shape
				N[1] = +(1 + r) / 2;
				N[2] = +(1 + s) / 2;
				N[3] = +(1 + t) / 2;
				N[0] = -(1 + r + s + t) / 2;
			}
			void Tetra4::gradient(double* B, const double* p) const
			{
				//gradient r
				B[2 + 0] = +0;
				B[3 + 0] = +0;
				B[0 + 0] = -1.0 / 2;
				B[1 + 0] = +1.0 / 2;
				//gradient s
				B[1 + 4] = +0;
				B[3 + 4] = +0;
				B[0 + 4] = -1.0 / 2;
				B[2 + 4] = +1.0 / 2;
				//gradient t
				B[1 + 8] = +0;
				B[2 + 8] = +0;
				B[0 + 8] = -1.0 / 2;
				B[3 + 8] = +1.0 / 2;
			}
		}
	}
}
//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad9.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Quad9::Quad9(void) : Quad(2)
			{
				return;
			}

			//destructor
			Quad9::~Quad9(void)
			{
				return;
			}

			//geometry
			uint32_t Quad9::vertices(void) const
			{
				return 9U;
			}

			//integration
			void Quad9::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[0] = +r * (1 - r) * s * (1 - s) / 4;
				N[1] = -r * (1 + r) * s * (1 - s) / 4;
				N[2] = +r * (1 + r) * s * (1 + s) / 4;
				N[3] = -r * (1 - r) * s * (1 + s) / 4;
				N[4] = -(1 - r) * (1 + r) * s * (1 - s) / 2;
				N[5] = +r * (1 + r) * (1 - s) * (1 + s) / 2;
				N[6] = +(1 - r) * (1 + r) * s * (1 + s) / 2;
				N[7] = -r * (1 - r) * (1 - s) * (1 + s) / 2;
				N[8] = (1 - r) * (1 + r) * (1 - s) * (1 + s);
			}
			void Quad9::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//gradient r
				B[4 + 0] = +r * s * (1 - s);
				B[6 + 0] = -r * s * (1 + s);
				B[8 + 0] = -2 * r * (1 - s) * (1 + s);
				B[0 + 0] = +(1 - 2 * r) * s * (1 - s) / 4;
				B[1 + 0] = -(1 + 2 * r) * s * (1 - s) / 4;
				B[2 + 0] = +(1 + 2 * r) * s * (1 + s) / 4;
				B[3 + 0] = -(1 - 2 * r) * s * (1 + s) / 4;
				B[5 + 0] = +(1 + 2 * r) * (1 - s) * (1 + s) / 2;
				B[7 + 0] = -(1 - 2 * r) * (1 - s) * (1 + s) / 2;
				//gradient s
				B[5 + 9] = -s * r * (1 + r);
				B[7 + 9] = +s * r * (1 - r);
				B[8 + 9] = -2 * s * (1 - r) * (1 + r);
				B[0 + 9] = +r * (1 - r) * (1 - 2 * s) / 4;
				B[1 + 9] = -r * (1 + r) * (1 - 2 * s) / 4;
				B[2 + 9] = +r * (1 + r) * (1 + 2 * s) / 4;
				B[3 + 9] = -r * (1 - r) * (1 + 2 * s) / 4;
				B[4 + 9] = -(1 - r) * (1 + r) * (1 - 2 * s) / 2;
				B[6 + 9] = +(1 - r) * (1 + r) * (1 + 2 * s) / 2;
			}
		}
	}
}
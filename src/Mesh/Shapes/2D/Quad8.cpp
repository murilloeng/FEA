//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad8.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Quad8::Quad8(void) : Quad(2)
			{
				return;
			}

			//destructor
			Quad8::~Quad8(void)
			{
				return;
			}

			//geometry
			uint32_t Quad8::vertices(void) const
			{
				return 8U;
			}

			//integration
			void Quad8::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[4] = (1 - r * r) * (1 - s) * (1 - 3 * s) / 8;
				N[5] = (1 + r) * (1 + 3 * r) * (1 - s * s) / 8;
				N[6] = (1 - r * r) * (1 + s) * (1 + 3 * s) / 8;
				N[7] = (1 - r) * (1 - 3 * r) * (1 - s * s) / 8;
				N[0] = (1 - r) * (1 - s) * (1 + r + s + 3 * r * s) / 8;
				N[1] = (1 + r) * (1 - s) * (1 - r + s - 3 * r * s) / 8;
				N[2] = (1 + r) * (1 + s) * (1 - r - s + 3 * r * s) / 8;
				N[3] = (1 - r) * (1 + s) * (1 + r - s - 3 * r * s) / 8;
			}
			void Quad8::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//gradient r
				B[5 + 0] = +(3 * r + 2) * (1 - s * s) / 4;
				B[7 + 0] = +(3 * r - 2) * (1 - s * s) / 4;
				B[4 + 0] = -r * (1 - s) * (1 - 3 * s) / 4;
				B[6 + 0] = -r * (1 + s) * (1 + 3 * s) / 4;
				B[0 + 0] = -(1 - s) * (r - s + 3 * r * s) / 4;
				B[1 + 0] = -(1 - s) * (r + s + 3 * r * s) / 4;
				B[2 + 0] = -(1 + s) * (r - s - 3 * r * s) / 4;
				B[3 + 0] = -(1 + s) * (r + s - 3 * r * s) / 4;
				//gradient s
				B[4 + 8] = -(1 - r * r) * (3 * s - 2) / 4;
				B[6 + 8] = +(1 - r * r) * (3 * s + 2) / 4;
				B[5 + 8] = -(1 + r) * (3 * r + 1) * s / 4;
				B[7 + 8] = +(1 - r) * (3 * r - 1) * s / 4;
				B[0 + 8] = -(1 - r) * (s - r + 3 * r * s) / 4;
				B[1 + 8] = -(1 + r) * (s + r - 3 * r * s) / 4;
				B[2 + 8] = -(1 + r) * (s - r - 3 * r * s) / 4;
				B[3 + 8] = -(1 - r) * (s + r + 3 * r * s) / 4;
			}
		}
	}
}
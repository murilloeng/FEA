//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri10.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri10::Tri10(void) : Tri(3)
			{
				return;
			}

			//destructor
			Tri10::~Tri10(void)
			{
				return;
			}

			//geometry
			uint32_t Tri10::vertices(void) const
			{
				return 10U;
			}

			//integration
			void Tri10::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[9] = -27 * (1 + r) * (1 + s) * (r + s) / 8;
				N[1] = -(1 + r) * (1 - 3 * r) * (1 + 3 * r) / 16;
				N[2] = -(1 + s) * (1 - 3 * s) * (1 + 3 * s) / 16;
				N[4] = -9 * (1 + r) * (1 + 3 * r) * (r + s) / 16;
				N[5] = +9 * (1 + r) * (1 + 3 * r) * (1 + s) / 16;
				N[6] = +9 * (1 + r) * (1 + s) * (1 + 3 * s) / 16;
				N[7] = -9 * (1 + s) * (1 + 3 * s) * (r + s) / 16;
				N[3] = +9 * (1 + r) * (r + s) * (3 * r + 3 * s + 2) / 16;
				N[8] = +9 * (1 + s) * (r + s) * (3 * r + 3 * s + 2) / 16;
				N[0] = -(r + s) * (2 + 3 * r + 3 * s) * (4 + 3 * r + 3 * s) / 16;

			}
			void Tri10::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//gradient r
				B[2 +  0] = 0;
				B[5 +  0] = +9 * (2 + 3 * r) * (1 + s) / 8;
				B[1 +  0] = -(1 - 18 * r - 27 * r * r) / 16;
				B[6 +  0] = +9 * (1 + s) * (1 + 3 * s) / 16;
				B[7 +  0] = -9 * (1 + s) * (1 + 3 * s) / 16;
				B[9 +  0] = -27 * (1 + s) * (1 + 2 * r + s) / 8;
				B[8 +  0] = +9 * (1 + s) * (1 + 3 * r + 3 * s) / 8;
				B[4 +  0] = -9 * (1 + 8 * r + 4 * s + 9 * r * r + 6 * r * s) / 16;
				B[0 +  0] = -(8 + 36 * r + 36 * s + 27 * r * r + 54 * r * s + 27 * s * s) / 16;
				B[3 +  0] = +9 * (2 + 10 * r + 8 * s + 9 * r * r + 12 * r * s + 3 * s * s) / 16;
				//gradient r
				B[1 + 10] = 0;
				B[2 + 10] = (27 * s * s + 18 * s - 1) / 16;
				B[6 + 10] = +9 * (1 + r) * (2 + 3 * s) / 8;
				B[4 + 10] = -9 * (1 + r) * (1 + 3 * r) / 16;
				B[5 + 10] = +9 * (1 + r) * (1 + 3 * r) / 16;
				B[9 + 10] = -27 * (1 + r) * (1 + r + 2 * s) / 8;
				B[3 + 10] = +9 * (1 + r) * (1 + 3 * r + 3 * s) / 8;
				B[7 + 10] = -9 * (1 + 4 * r + 8 * s + 6 * r * s + 9 * s * s) / 16;
				B[0 + 10] = -(8 + 36 * r + 36 * s + 27 * r * r + 54 * r * s + 27 * s * s) / 16;
				B[8 + 10] = +9 * (2 + 8 * r + 10 * s + 3 * r * r + 12 * r * s + 9 * s * s) / 16;
			}
		}
	}
}
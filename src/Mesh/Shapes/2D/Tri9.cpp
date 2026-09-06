//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri9.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri9::Tri9(void) : Tri(3)
			{
				return;
			}

			//destructor
			Tri9::~Tri9(void)
			{
				return;
			}

			//geometry
			uint32_t Tri9::vertices(void) const
			{
				return 9U;
			}

			//integration
			void Tri9::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[7] = -3 * (1 + s) * (r + s) * (5 + 2 * r + 9 * s) / 16;
				N[8] = +3 * (1 + s) * (r + s) * (4 + 7 * r + 9 * s) / 16;
				N[3] = +3 * (1 + r) * (r + s) * (+4 + 9 * r + 7 * s) / 16;
				N[4] = -3 * (1 + r) * (r + s) * (+5 + 9 * r + 2 * s) / 16;
				N[5] = +3 * (1 + r) * (1 + s) * (+3 + 7 * r - 2 * s) / 16;
				N[6] = -3 * (1 + r) * (1 + s) * (-3 + 2 * r - 7 * s) / 16;
				N[1] = -(1 + r) * (1 + 6 * r + 6 * s - 9 * r * r + 6 * r * s + 6 * s * s) / 16;
				N[2] = -(1 + s) * (1 + 6 * r + 6 * s + 6 * r * r + 6 * r * s - 9 * s * s) / 16;
				N[0] = -(r + s) * (14 + 24 * r + 24 * s + 9 * r * r + 24 * r * s + 9 * s * s) / 16;
			}
			void Tri9::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//gradient r
				B[5 +  0] = +3 * (1 + s) * (7 * r - s + 5) / 8;
				B[2 +  0] = -3 * (1 + s) * (1 + 2 * r + s) / 8;
				B[8 +  0] = +3 * (1 + s) * (2 + 7 * r + 8 * s) / 8;
				B[6 +  0] = -3 * (1 + s) * (-1 + 4 * r -  7 * s) / 16;
				B[7 +  0] = -3 * (1 + s) * (+5 + 4 * r + 11 * s) / 16;
				B[1 +  0] = +(27 * r * r - 12 * r * s +  6 * r -  6 * s * s - 12 * s -  7) / 16;
				B[0 +  0] = -(27 * r * r + 66 * r * s + 48 * r + 33 * s * s + 48 * s + 14) / 16;
				B[3 +  0] = +3 * (27 * r * r + 32 * r * s + 7 * s * s + 26 * r + 20 * s + 4) / 16;
				B[4 +  0] = -3 * (27 * r * r + 22 * r * s + 2 * s * s + 28 * r + 16 * s + 5) / 16;
				//gradient s
				B[1 + 9] = -3 * (1 + r) * (r + 2 * s + 1) / 8;
				B[6 + 9] = -3 * (1 + r) * (r - 7 * s - 5) / 8;
				B[3 + 9] = +3 * (1 + r) * (8 * r + 7 * s + 2) / 8;
				B[5 + 9] = +3 * (1 + r) * ( 7 * r - 4 * s + 1) / 16;
				B[4 + 9] = -3 * (1 + r) * (11 * r + 4 * s + 5) / 16;
				B[2 + 9] = -( 6 * r * r + 12 * r * s - 27 * s * s + 12 * r -  6 * s +  7) / 16;
				B[0 + 9] = -(33 * r * r + 66 * r * s + 27 * s * s + 48 * r + 48 * s + 14) / 16;
				B[8 + 9] = +3 * (7 * r * r + 32 * r * s + 27 * s * s + 20 * r + 26 * s + 4) / 16;
				B[7 + 9] = -3 * (2 * r * r + 22 * r * s + 27 * s * s + 16 * r + 28 * s + 5) / 16;
			}
		}
	}
}
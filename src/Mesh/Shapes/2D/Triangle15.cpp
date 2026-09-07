//FEA
#include "FEA/inc/Mesh/Shapes/2D/Triangle15.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Triangle15::Triangle15(void) : Triangle(3)
			{
				return;
			}

			//destructor
			Triangle15::~Triangle15(void)
			{
				return;
			}

			//geometry
			uint32_t Triangle15::vertices(void) const
			{
				return 15U;
			}

			//integration
			void Triangle15::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[ 1] = r * (1 + r) * (2 * r - 1) * (1 + 2 * r) / 6;
				N[ 2] = s * (1 + s) * (2 * s - 1) * (1 + 2 * s) / 6;
				N[ 5] = -4 * r * (1 + r) * (r + s) * (1 + 2 * r) / 3;
				N[ 6] = +4 * r * (1 + r) * (1 + s) * (1 + 2 * r) / 3;
				N[ 8] = +4 * s * (1 + r) * (1 + s) * (1 + 2 * s) / 3;
				N[ 9] = -4 * s * (1 + s) * (r + s) * (1 + 2 * s) / 3;
				N[ 7] = (1 + r) * (1 + 2 * r) * (1 + s) * (1 + 2 * s);
				N[13] = -4 * (1 + r) * (r + s) * (1 + 2 * r) * (1 + s);
				N[14] = -4 * (1 + r) * (r + s) * (1 + s) * (1 + 2 * s);
				N[ 4] = (1 + r) * (1 + 2 * r) * (r + s) * (1 + 2 * r + 2 * s);
				N[10] = (1 + s) * (1 + 2 * s) * (r + s) * (1 + 2 * r + 2 * s);
				N[12] = +4 * (1 + r) * (1 + s) * (r + s) * (1 + 2 * r + 2 * s);
				N[ 3] = -4 * (1 + r) * (r + s) * (1 + r + s) * (1 + 2 * r + 2 * s) / 3;
				N[11] = -4 * (r + s) * (1 + s) * (1 + r + s) * (1 + 2 * r + 2 * s) / 3;
				N[ 0] = (r + s) * (1 + r + s) * (1 + 2 * r + 2 * s) * (3 + 2 * r + 2 * s) / 6;
			}
			void Triangle15::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//gradient r
				B[ 2 +  0] = 0;
				B[ 8 +  0] = +4 * s * (1 + s) * (1 + 2 * s) / 3;
				B[ 9 +  0] = -4 * s * (1 + s) * (1 + 2 * s) / 3;
				B[ 7 +  0] = (4 * r + 3) * (1 + s) * (1 + 2 * s);
				B[ 1 +  0] = (4 * r + 1) * (4 * r * r + 2 * r - 1) / 6;
				B[ 6 +  0] = +4 * (1 + s) * (6 * r * r + 6 * r + 1) / 3;
				B[10 +  0] = (1 + s) * (1 + 2 * s) * (4 * r + 4 * s + 1);
				B[14 +  0] = -4 * (1 + s) * (1 + 2 * s) * (2 * r + s + 1);
				B[13 +  0] = -4 * (1 + s) * (6 * r * r + 4 * r * s + 6 * r + 3 * s + 1);
				B[ 4 +  0] = (2 * r + s + 1) * (8 * r * r + 8 * r * s + 8 * r + 6 * s + 1);
				B[12 +  0] = +4 * (1 + s) * (6 * r * r + 8 * r * s + 6 * r + 2 * s * s + 5 * s + 1);
				B[11 +  0] = -4 * (1 + s) * (6 * r * r + 12 * r * s + 6 * r + 6 * s * s + 6 * s + 1) / 3;
				B[ 5 +  0] = -4 * (8 * r * r * r + 6 * r * r * s + 9 * r * r + 6 * r * s + 2 * r + s) / 3;
				B[ 0 +  0] = (4 * r + 4 * s + 3) * (4 * r * r + 8 * r * s + 6 * r + 4 * s * s + 6 * s + 1) / 6;
				B[ 3 +  0] = -4 * (8 * r * r * r + 18 * r * r * s + 15 * r * r + 12 * r * s * s + 24 * r * s + 8 * r + 2 * s * s * s + 9 * s * s + 7 * s + 1) / 3;
				//gradient s
				B[ 1 + 15] = 0;
				B[ 5 + 15] = -4 * r * (1 + r) * (1 + 2 * r) / 3;
				B[ 6 + 15] = +4 * r * (1 + r) * (1 + 2 * r) / 3;
				B[ 7 + 15] = (1 + r) * (1 + 2 * r) * (4 * s + 3);
				B[ 2 + 15] = (4 * s + 1) * (4 * s * s + 2 * s - 1) / 6;
				B[ 8 + 15] = +4 * (1 + r) * (6 * s * s + 6 * s + 1) / 3;
				B[ 4 + 15] = (1 + r) * (1 + 2 * r) * (4 * r + 4 * s + 1);
				B[13 + 15] = -4 * (1 + r) * (1 + 2 * r) * (r + 2 * s + 1);
				B[14 + 15] = -4 * (1 + r) * (4 * r * s + 3 * r + 6 * s * s + 6 * s + 1);
				B[10 + 15] = (r + 2 * s + 1) * (8 * r * s + 6 * r + 8 * s * s + 8 * s + 1);
				B[12 + 15] = +4 * (1 + r) * (2 * r * r + 8 * r * s + 5 * r + 6 * s * s + 6 * s + 1);
				B[ 3 + 15] = -4 * (1 + r) * (6 * r * r + 12 * r * s + 6 * r + 6 * s * s + 6 * s + 1) / 3;
				B[ 9 + 15] = -4 * (6 * r * s * s + 6 * r * s + r + 8 * s * s * s + 9 * s * s + 2 * s) / 3;
				B[ 0 + 15] = (4 * r + 4 * s + 3) * (4 * r * r + 8 * r * s + 6 * r + 4 * s * s + 6 * s + 1) / 6;
				B[11 + 15] = -4 * (2 * r * r * r + 12 * r * r * s + 9 * r * r + 18 * r * s * s + 24 * r * s + 7 * r + 8 * s * s * s + 15 * s * s + 8 * s + 1) / 3;
			}
		}
	}
}
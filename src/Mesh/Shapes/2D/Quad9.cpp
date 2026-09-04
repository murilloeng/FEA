//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad9.hpp"

//static
static const double A[] = {
	+0, +0, +0, +0, +0, +0, +0, +0, +4, 
	+0, +0, +0, +0, +0, +2, +0, -2, +0, 
	+0, +0, +0, +0, -2, +0, +2, +0, +0,
	+0, +0, +0, +0, +0, +2, +0, +2, -4, 
	+1, -1, +1, -1, +0, +0, +0, +0, +0, 
	+0, +0, +0, +0, +2, +0, +2, +0, -4, 
	-1, -1, +1, +1, +2, +0, -2, +0, +0, 
	-1, +1, +1, -1, +0, -2, +0, +2, +0, 
	+1, +1, +1, +1, -2, -2, -2, -2, +4
};

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
				N[8] = (1 - r * r) * (1 - s * s);
				N[0] = r * (r - 1) * s * (s - 1) / 4;
				N[1] = r * (r + 1) * s * (s - 1) / 4;
				N[2] = r * (r + 1) * s * (s + 1) / 4;
				N[3] = r * (r - 1) * s * (s + 1) / 4;
				N[4] = (1 - r * r) * s * (s - 1) / 2;
				N[5] = r * (r + 1) * (1 - s * s) / 2;
				N[6] = (1 - r * r) * s * (s + 1) / 2;
				N[7] = r * (r - 1) * (1 - s * s) / 2;
			}
			void Quad9::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//gradient r
				B[4 + 0] = -r * s * (s - 1);
				B[6 + 0] = -r * s * (s + 1);
				B[8 + 0] = -2 * r * (1 - s * s);
				B[0 + 0] = (2 * r - 1) * s * (s - 1) / 4;
				B[1 + 0] = (2 * r + 1) * s * (s - 1) / 4;
				B[2 + 0] = (2 * r + 1) * s * (s + 1) / 4;
				B[3 + 0] = (2 * r - 1) * s * (s + 1) / 4;
				B[5 + 0] = (2 * r + 1) * (1 - s * s) / 2;
				B[7 + 0] = (2 * r - 1) * (1 - s * s) / 2;
				//gradient s
				B[5 + 9] = -s * r * (r + 1);
				B[7 + 9] = -s * r * (r - 1);
				B[8 + 9] = -2 * s * (1 - r * r);
				B[0 + 9] = r * (r - 1) * (2 * s - 1) / 4;
				B[1 + 9] = r * (r + 1) * (2 * s - 1) / 4;
				B[2 + 9] = r * (r + 1) * (2 * s + 1) / 4;
				B[3 + 9] = r * (r - 1) * (2 * s + 1) / 4;
				B[4 + 9] = (1 - r * r) * (2 * s - 1) / 2;
				B[6 + 9] = (1 - r * r) * (2 * s + 1) / 2;
			}
		}
	}
}
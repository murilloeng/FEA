//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad9.hpp"

//static
static const double A[] = {
	+0, +0, +0, +0, +1, +0, -1, -1, +1,
	+0, +0, +0, +0, -1, +0, -1, +1, +1,
	+0, +0, +0, +0, +1, +0, +1, +1, +1,
	+0, +0, +0, +0, -1, +0, +1, -1, +1,
	+0, +0, -2, +0, +0, +2, +2, +0, -2,
	+0, +2, +0, +2, +0, +0, +0, -2, -2,
	+0, +0, +2, +0, +0, +2, -2, +0, -2,
	+0, -2, +0, +2, +0, +0, +0, +2, -2,
	+4, +0, +0, -4, +0, -4, +0, +0, +4
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Quad9::Quad9(void) : Quadrangle(2)
			{
				return;
			}

			//destructor
			Quad9::~Quad9(void)
			{
				return;
			}

			//integration
			void Quad9::function(double* N, const double* s) const
			{
				//data
				const double v[] = {
					1, s[0], s[1], s[0] * s[0], s[0] * s[1], s[1] * s[1], 
					s[0] * s[0] * s[1], s[0] * s[1] * s[1], s[0] * s[0] * s[1] * s[1]
				};
				//shape
				for(uint32_t i = 0; i < 9; i++)
				{
					N[i] = 0;
					for (uint32_t j = 0; j < 9; j++)
					{
						N[i] += A[9 * i + j] / 4 * v[j];
					}
				}
			}
			void Quad9::gradient(double* B, const double* s) const
			{
				//data
				const double dv1[] = {0, 1, 0, 2 * s[0], s[1], 0, 2 * s[0] * s[1], s[1] * s[1], 2 * s[0] * s[1] * s[1]};
				const double dv2[] = {0, 0, 1, 0, s[0], 2 * s[1], s[0] * s[0], 2 * s[0] * s[1], 2 * s[0] * s[0] * s[1]};
				//gradient
				for(uint32_t i = 0; i < 9; i++)
				{
					B[9 * 0 + i] = 0;
					B[9 * 1 + i] = 0;
					for(uint32_t j = 0; j < 9; j++)
					{
						B[9 * 0 + i] += A[9 * i + j] / 4 * dv1[j];
						B[9 * 1 + i] += A[9 * i + j] / 4 * dv2[j];
					}
				}
			}
		}
	}
}
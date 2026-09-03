//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri9.hpp"

//static
static const double A[] = {
	+0, -14, -14, -24, -48, -24,  -9, -33, -33,  -9,
	-1,  -7,  -6,  +3, -12,  -6,  +9,  -6,  -6,  +0,
	-1,  -6,  -7,  -6, -12,  +3,  +0,  -6,  -6,  +9,
	+0, +12, +12, +39, +60, +21, +27, +48, +21,  +0,
	+0, -15, -15, -42, -48,  -6, -27, -33,  -6,  +0,
	+9, +30,  +3, +21, +24,  -6,  +0, +21,  -6,  +0,
	+9,  +3, +30,  -6, +24, +21,  +0,  -6, +21,  +0,
	+0, -15, -15,  -6, -48, -42,  +0,  -6, -33, -27,
	+0, +12, +12, +21, +60, +39,  +0, +21, +48, +27
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri9::Tri9(void) : Triangle(3)
			{
				return;
			}

			//destructor
			Tri9::~Tri9(void)
			{
				return;
			}

			//integration
			void Tri9::function(double* N, const double* s) const
			{
				//data
				const double v[] = {
					1, s[0], s[1], s[0] * s[0], s[0] * s[1], s[1] * s[1], 
					s[0] * s[0] * s[0], s[0] * s[0] * s[1], s[0] * s[1] * s[1], s[1] * s[1] * s[1]
				};
				//shape
				for (uint32_t i = 0; i < 9; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 10; j++)
					{
						N[i] += A[10 * i + j] / 16 * v[j];
					}
				}
			}
			void Tri9::gradient(double* B, const double* s) const
			{
				//data
				const double dv1[] = {
					0, 1, 0, 2 * s[0], s[1], 0, 3 * s[0] * s[0], 2 * s[0] * s[1], s[1] * s[1], 0
				};
				const double dv2[] = {
					0, 0, 1, 0, s[0], 2 * s[1], 0, s[0] * s[0], 2 * s[0] * s[1], 3 * s[1] * s[1]
				};
				//gradient
				for(uint32_t i = 0; i < 9; i++)
				{
					B[9 * 0 + i] = 0;
					B[9 * 1 + i] = 0;
					for(uint32_t j = 0; j < 10; j++)
					{
						B[9 * 0 + i] += A[10 * i + j] / 16 * dv1[j];
						B[9 * 1 + i] += A[10 * i + j] / 16 * dv2[j];
					}
				}
			}
		}
	}
}
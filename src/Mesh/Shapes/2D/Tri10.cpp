//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri10.hpp"

//static
static const double A[] = {
	+0,  -8,  -8, -18,  -36, -18,  -9, -27, -27,  -9,
	-1,  -1,  +0,  +9,   +0,  +0,  +9,  +0,  +0,  +0,
	-1,  +0,  -1,  +0,   +0,  +9,  +0,  +0,  +0,  +9,
	+0, +18, +18, +45,  +72, +27, +27, +54, +27,  +0,
	+0,  -9,  -9, -36,  -36,  +0, -27, -27,  +0,  +0,
	+9, +36,  +9, +27,  +36,  +0,  +0, +27,  +0,  +0,
	+9,  +9, +36,  +0,  +36, +27,  +0,  +0, +27,  +0,
	+0,  -9,  -9,  +0,  -36, -36,  +0,  +0, -27, -27,
	+0, +18, +18, +27,  +72, +45,  +0, +27, +54, +27,
	+0, -54, -54, -54, -108, -54,  +0, -54, -54,  +0
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri10::Tri10(void) : Triangle(3)
			{
				return;
			}

			//destructor
			Tri10::~Tri10(void)
			{
				return;
			}

			//integration
			void Tri10::function(double* N, const double* s) const
			{
				//data
				const double v[] = {
					1, s[0], s[1], s[0] * s[0], s[0] * s[1], s[1] * s[1], 
					s[0] * s[0] * s[0], s[0] * s[0] * s[1], s[0] * s[1] * s[1], s[1] * s[1] * s[1]
				};
				//shape
				for(uint32_t i = 0; i < 10; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 10; j++)
					{
						N[i] += A[10 * i + j] / 16 * v[j];
					}
				}
			}
			void Tri10::gradient(double* B, const double* s) const
			{
				//data
				const double dv1[] = {
					0, 1, 0, 2 * s[0], s[1], 0, 3 * s[0] * s[0], 2 * s[0] * s[1], s[1] * s[1], 0
				};
				const double dv2[] = {
					0, 0, 1, 0, s[0], 2 * s[1], 0, s[0] * s[0], 2 * s[0] * s[1], 3 * s[1] * s[1]
				};
				//gradient
				for(uint32_t i = 0; i < 10; i++)
				{
					B[10 * 0 + i] = 0;
					B[10 * 1 + i] = 0;
					for(uint32_t j = 0; j < 10; j++)
					{
						B[10 * 0 + i] += A[10 * i + j] / 16 * dv1[j];
						B[10 * 1 + i] += A[10 * i + j] / 16 * dv2[j];
					}
				}
			}
		}
	}
}
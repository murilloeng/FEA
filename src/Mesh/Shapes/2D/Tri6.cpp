//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri6.hpp"

//static
static const double A[] = {
	+0, +1, +1, +1, +2, +1,
	+0, +1, +0, +1, +0, +0,
	+0, +0, +1, +0, +0, +1,
	+0, -2, -2, -2, -2, +0,
	+2, +2, +2, +0, +2, +0,
	+0, -2, -2, +0, -2, -2
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri6::Tri6(void) : Triangle(2)
			{
				return;
			}

			//destructor
			Tri6::~Tri6(void)
			{
				return;
			}

			//integration
			void Tri6::function(double* N, const double* s) const
			{
				//data
				const double v[] = {
					1, s[0], s[1], s[0] * s[0], s[0] * s[1], s[1] * s[1]
				};
				//shape
				for(uint32_t i = 0; i < 6; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 6; j++)
					{
						N[i] += A[6 * i + j] / 2 * v[j];
					}
				}
			}
			void Tri6::gradient(double* B, const double* s) const
			{
				//data
				const double dv1[] = {
					0, 1, 0, 2 * s[0], s[1], 0
				};
				const double dv2[] = {
					0, 0, 1, 0, s[0], 2 * s[1]
				};
				//gradient
				for(uint32_t i = 0; i < 6; i++)
				{
					B[6 * 0 + i] = 0;
					B[6 * 1 + i] = 0;
					for(uint32_t j = 0; j < 6; j++)
					{
						B[6 * 0 + i] += A[6 * i + j] / 2 * dv1[j];
						B[6 * 1 + i] += A[6 * i + j] / 2 * dv2[j];
					}
				}
			}
		}
	}
}
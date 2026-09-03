//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad8.hpp"

//static
static const double A[] = {
	+1, +0, +0, -1, +2, -1, -2, -2, +3,
	+1, +0, +0, -1, -2, -1, -2, +2, +3,
	+1, +0, +0, -1, +2, -1, +2, +2, +3,
	+1, +0, +0, -1, -2, -1, +2, -2, +3,
	+1, +0, -4, -1, +0, +3, +4, +0, -3,
	+1, +4, +0, +3, +0, -1, +0, -4, -3,
	+1, +0, +4, -1, +0, +3, -4, +0, -3,
	+1, -4, +0, +3, +0, -1, +0, +4, -3
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Quad8::Quad8(void) : Quadrangle(2)
			{
				return;
			}

			//destructor
			Quad8::~Quad8(void)
			{
				return;
			}

			//integration
			void Quad8::function(double* N, const double* s) const
			{
				//data
				const double v[] = {
					1, s[0], s[1], s[0] * s[0], s[0] * s[1], s[1] * s[1], 
					s[0] * s[0] * s[1], s[0] * s[1] * s[1], s[0] * s[0] * s[1] * s[1]
				};
				//shape
				for(uint32_t i = 0; i < 8; i++)
				{
					N[i] = 0;
					for (uint32_t j = 0; j < 9; j++)
					{
						N[i] += A[8 * i + j] / 8 * v[j];
					}
				}
			}
			void Quad8::gradient(double* B, const double* s) const
			{
				//data
				const double dv1[] = {0, 1, 0, 2 * s[0], s[1], 0, 2 * s[0] * s[1], s[1] * s[1], 2 * s[0] * s[1] * s[1]};
				const double dv2[] = {0, 0, 1, 0, s[0], 2 * s[1], s[0] * s[0], 2 * s[0] * s[1], 2 * s[0] * s[0] * s[1]};
				//gradient
				for(uint32_t i = 0; i < 8; i++)
				{
					B[8 * 0 + i] = 0;
					B[8 * 1 + i] = 0;
					for(uint32_t j = 0; j < 9; j++)
					{
						B[8 * 0 + i] += A[8 * i + j] / 8 * dv1[j];
						B[8 * 1 + i] += A[8 * i + j] / 8 * dv2[j];
					}
				}
			}
		}
	}
}
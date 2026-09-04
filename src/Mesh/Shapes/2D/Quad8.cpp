//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad8.hpp"

//static
static const double A[] = {
	+1, +1, +1, +1, +1, +1, +1, +1,
	+0, +0, +0, +0, +0, +4, +0, -4,
	+0, +0, +0, +0, -4, +0, +4, +0,
	-1, -1, -1, -1, -1, +3, -1, +3,
	+2, -2, +2, -2, +0, +0, +0, +0,
	-1, -1, -1, -1, +3, -1, +3, -1,
	-2, -2, +2, +2, +4, +0, -4, +0,
	-2, +2, +2, -2, +0, -4, +0, +4,
	+3, +3, +3, +3, -3, -3, -3, -3
};

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
				const double v[] = {1, r, s, r * r, r * s, s * s, r * r * s, r * s * s, r * r * s * s};
				//shape
				for(uint32_t i = 0; i < 8; i++)
				{
					N[i] = 0;
					for (uint32_t j = 0; j < 9; j++)
					{
						N[i] += A[i + 8 * j] / 8 * v[j];
					}
				}
			}
			void Quad8::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double dv[] = {
					0, 1, 0, 2 * r, s, 0, 2 * r * s, s * s, 2 * r * s * s,
					0, 0, 1, 0, r, 2 * s, r * r, 2 * r * s, 2 * r * r * s
				};
				//gradient
				for(uint32_t i = 0; i < 8; i++)
				{
					B[i + 0] = B[i + 8] = 0;
					for(uint32_t j = 0; j < 9; j++)
					{
						B[i + 0] += A[i + 8 * j] / 8 * dv[j + 0];
						B[i + 8] += A[i + 8 * j] / 8 * dv[j + 8];
					}
				}
			}
		}
	}
}
//FEA
#include "FEA/inc/Mesh/Shapes/3D/Brick8.hpp"

//static
static const double A[] = {
	+1, +1, +1, +1, +1, +1, +1, +1,
	-1, +1, +1, -1, -1, +1, +1, -1,
	-1, -1, +1, +1, -1, -1, +1, +1,
	-1, -1, -1, -1, +1, +1, +1, +1,
	+1, -1, +1, -1, +1, -1, +1, -1,
	+1, -1, -1, +1, -1, +1, +1, -1,
	+1, +1, -1, -1, -1, -1, +1, +1,
	-1, +1, -1, +1, +1, -1, +1, -1
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Brick8::Brick8(void) : Brick(1)
			{
				return;
			}

			//destructor
			Brick8::~Brick8(void)
			{
				return;
			}

			//geometry
			uint32_t Brick8::vertices(void) const
			{
				return 8U;
			}

			//integration
			void Brick8::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				const double v[] = {1, r, s, t, r * s, r * t, s * t, r * s * t};
				//shape
				for(uint32_t i = 0; i < 8; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 8; j++)
					{
						N[i] += A[i + 8 * j] / 8 * v[j];
					}
				}
			}
			void Brick8::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				const double dv[] = {
					0, 1, 0, 0, s, t, 0, s * t,
					0, 0, 1, 0, r, 0, t, r * t,
					0, 0, 0, 1, 0, r, s, r * s
				};
				//shape
				for(uint32_t i = 0; i < 8; i++)
				{
					B[i +  0] = 0;
					B[i +  8] = 0;
					B[i + 16] = 0;
					for(uint32_t j = 0; j < 8; j++)
					{
						B[i +  0] += A[i + 8 * j] / 8 * dv[j +  0];
						B[i +  8] += A[i + 8 * j] / 8 * dv[j +  8];
						B[i + 16] += A[i + 8 * j] / 8 * dv[j + 16];
					}
				}
			}
		}
	}
}
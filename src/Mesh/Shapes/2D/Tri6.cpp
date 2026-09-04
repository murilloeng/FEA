//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri6.hpp"

//static
static const double A[] = {
	+0, +0, +0, +0, +2, +0,
	+1, +1, +0, -2, +2, -2,
	+1, +0, +1, -2, +2, -2,
	+1, +1, +0, -2, +0, +0,
	+2, +0, +0, -2, +2, -2,
	+1, +0, +1, +0, +0, -2
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri6::Tri6(void) : Tri(2)
			{
				return;
			}

			//destructor
			Tri6::~Tri6(void)
			{
				return;
			}

			//integration
			void Tri6::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double v[] = {1, r, s, r * r, r * s, s * s};
				//shape
				for(uint32_t i = 0; i < 6; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 6; j++)
					{
						N[i] += A[i + 6 * j] / 2 * v[j];
					}
				}
			}
			void Tri6::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double dv[] = {
					0, 1, 0, 2 * r, s, 0,
					0, 0, 1, 0, r, 2 * s
				};
				//gradient
				for(uint32_t i = 0; i < 6; i++)
				{
					B[i + 0] = B[i + 6] = 0;
					for(uint32_t j = 0; j < 6; j++)
					{
						B[i + 0] += A[i + 6 * j] / 2 * dv[j + 0];
						B[i + 6] += A[i + 6 * j] / 2 * dv[j + 6];
					}
				}
			}
		}
	}
}
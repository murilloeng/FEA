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

			//integration
			void Quad9::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double v[] = {1, r, s, r * r, r * s, s * s, r * r * s, r * s * s, r * r * s * s};
				//shape
				for(uint32_t i = 0; i < 9; i++)
				{
					N[i] = 0;
					for (uint32_t j = 0; j < 9; j++)
					{
						N[i] += A[i + 9 * j] / 4 * v[j];
					}
				}
			}
			void Quad9::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double dv[] = {
					0, 1, 0, 2 * r, s, 0, 2 * r * s, s * s, 2 * r * s * s,
					0, 0, 1, 0, r, 2 * s, r * r, 2 * r * s, 2 * r * r * s
				};
				//gradient
				for(uint32_t i = 0; i < 9; i++)
				{
					B[i + 0] = B[i + 9] = 0;
					for(uint32_t j = 0; j < 9; j++)
					{
						B[i + 0] += A[i + 9 * j] / 4 * dv[j + 0];
						B[i + 9] += A[i + 9 * j] / 4 * dv[j + 0];
					}
				}
			}
		}
	}
}
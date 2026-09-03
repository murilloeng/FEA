//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad4.hpp"

//static
static const double A[] = {
	+1, -1, -1, +1,
	+1, +1, -1, -1,
	+1, +1, +1, +1,
	+1, -1, +1, -1
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Quad4::Quad4(void) : Quadrangle(2)
			{
				return;
			}

			//destructor
			Quad4::~Quad4(void)
			{
				return;
			}

			//integration
			void Quad4::function(double* N, const double* s) const
			{
				//data
				const double v[] = {1, s[0], s[1], s[0] * s[1]};
				//shape
				for(uint32_t i = 0; i < 4; i++)
				{
					N[i] = 0;
					for (uint32_t j = 0; j < 4; j++)
					{
						N[i] += A[4 * i + j] / 4 * v[j];
					}
				}
			}
			void Quad4::gradient(double* B, const double* s) const
			{
				//data
				const double dv1[] = {0, 1, 0, s[1]};
				const double dv2[] = {0, 0, 1, s[0]};
				//gradient
				for(uint32_t i = 0; i < 4; i++)
				{
					B[4 * 0 + i] = 0;
					B[4 * 1 + i] = 0;
					for(uint32_t j = 0; j < 4; j++)
					{
						B[4 * 0 + i] += A[4 * i + j] / 4 * dv1[j];
						B[4 * 1 + i] += A[4 * i + j] / 4 * dv2[j];
					}
				}
			}
		}
	}
}
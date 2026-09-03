//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri3.hpp"

//static
static const double A[] = {
	+0, -1, -1, 
	+1, +1, +0, 
	+1, +0, +1
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri3::Tri3(void) : Triangle(1)
			{
				return;
			}

			//destructor
			Tri3::~Tri3(void)
			{
				return;
			}

			//integration
			void Tri3::function(double* N, const double* s) const
			{
				//data
				const double v[] = {1, s[0], s[1]};
				//shape
				for(uint32_t i = 0; i < 3; i++)
				{
					N[i] = 0;
					for (uint32_t j = 0; j < 3; j++)
					{
						N[i] += A[3 * i + j] / 2 * v[j];
					}
				}
			}
			void Tri3::gradient(double* B, const double* s) const
			{
				//data
				const double dv1[] = {0, 1, 0};
				const double dv2[] = {0, 0, 1};
				//gradient
				for(uint32_t i = 0; i < 3; i++)
				{
					B[3 * 0 + i] = 0;
					B[3 * 1 + i] = 0;
					for(uint32_t j = 0; j < 3; j++)
					{
						B[3 * 0 + i] += A[3 * i + j] / 2 * dv1[j];
						B[3 * 1 + i] += A[3 * i + j] / 2 * dv2[j];
					}
				}
			}
		}
	}
}
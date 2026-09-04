//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri3.hpp"

//static
static const double A[] = {
	+0, +1, +1,
	-1, +1, +0, 
	-1, +0, +1
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri3::Tri3(void) : Tri(1)
			{
				return;
			}

			//destructor
			Tri3::~Tri3(void)
			{
				return;
			}

			//geometry
			uint32_t Tri3::vertices(void) const
			{
				return 3U;
			}

			//integration
			void Tri3::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double v[] = {1, r, s};
				//shape
				for(uint32_t i = 0; i < 3; i++)
				{
					N[i] = 0;
					for (uint32_t j = 0; j < 3; j++)
					{
						N[i] += A[i + 3 * j] / 2 * v[j];
					}
				}
			}
			void Tri3::gradient(double* B, const double* p) const
			{
				//data
				const double dv[] = {
					0, 1, 0,
					0, 0, 1
				};
				//gradient
				for(uint32_t i = 0; i < 3; i++)
				{
					B[i + 0] = B[i + 3] = 0;
					for(uint32_t j = 0; j < 3; j++)
					{
						B[i + 0] += A[i + 3 * j] / 2 * dv[j + 0];
						B[i + 3] += A[i + 3 * j] / 2 * dv[j + 3];
					}
				}
			}
		}
	}
}
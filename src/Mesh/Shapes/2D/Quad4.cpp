//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quad4.hpp"

//static
static const double A[] = {
	+1, +1, +1, +1, 
	-1, +1, +1, -1, 
	-1, -1, +1, +1, 
	+1, -1, +1, -1
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Quad4::Quad4(void) : Quad(2)
			{
				return;
			}

			//destructor
			Quad4::~Quad4(void)
			{
				return;
			}

			//integration
			void Quad4::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double v[] = {1, r, s, r * s};
				//shape
				for(uint32_t i = 0; i < 4; i++)
				{
					N[i] = 0;
					for (uint32_t j = 0; j < 4; j++)
					{
						N[i] += A[i + 4 * j] / 4 * v[j];
					}
				}
			}
			void Quad4::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double dv[] = {
					0, 1, 0, s,
					0, 0, 1, r
				};
				//gradient
				for(uint32_t i = 0; i < 4; i++)
				{
					B[i + 0] = B[i + 4] = 0;
					for(uint32_t j = 0; j < 4; j++)
					{
						B[i + 0] += A[i + 4 * j] / 4 * dv[j + 0];
						B[i + 4] += A[i + 4 * j] / 4 * dv[j + 4];
					}
				}
			}
		}
	}
}
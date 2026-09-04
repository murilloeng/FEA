//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line3.hpp"

//static
static const double A[] = {
	+0, +0, +2, 
	-1, +1, +0, 
	+1, +1, -2
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line3::Line3(void) : Line(2)
			{
				return;
			}

			//destructor
			Line3::~Line3(void)
			{
				return;
			}

			//geometry
			uint32_t Line3::vertices(void) const
			{
				return 3U;
			}

			//shape
			void Line3::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double v[] = {1, r, r * r};
				//shape
				for(uint32_t i = 0; i < 3; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 3; j++)
					{
						N[i] += A[i + 3 * j] / 2 * v[j];
					}
				}
			}
			void Line3::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double dv[] = {0, 1, 2 * r};
				//shape
				for(uint32_t i = 0; i < 3; i++)
				{
					B[i] = 0;
					for(uint32_t j = 0; j < 3; j++)
					{
						B[i] += A[i + 3 * j] / 2 * dv[j];
					}
				}
			}
		}
	}
}
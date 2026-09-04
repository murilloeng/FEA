//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line2.hpp"

//static
static const double A[] = {
	+1.0, +1.0,
	-1.0, +1.0
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line2::Line2(void) : Line(2)
			{
				return;
			}

			//destructor
			Line2::~Line2(void)
			{
				return;
			}

			//geometry
			uint32_t Line2::vertices(void) const
			{
				return 2U;
			}

			//shape
			void Line2::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double v[] = {1, r};
				//shape
				for(uint32_t i = 0; i < 2; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 2; j++)
					{
						N[i] += A[i + 2 * j] / 2 * v[j];
					}
				}
			}
			void Line2::gradient(double* B, const double* p) const
			{
				//data
				const double dv[] = {0, 1};
				//gradient
				for(uint32_t i = 0; i < 2; i++)
				{
					B[i] = 0;
					for(uint32_t j = 0; j < 2; j++)
					{
						B[i] += A[i + 2 * j] / 2 * dv[j];
					}
				}
			}
		}
	}
}
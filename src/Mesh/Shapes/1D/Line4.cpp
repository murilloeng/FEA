//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line4.hpp"

//static
static const double A[] = {
	-1, -1,  +9,  +9, 
	+1, -1, -27, +27, 
	+9, +9,  -9,  -9, 
	-9, +9, +27, -27
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line4::Line4(void) : Line(2)
			{
				return;
			}

			//destructor
			Line4::~Line4(void)
			{
				return;
			}

			//geometry
			uint32_t Line4::vertices(void) const
			{
				return 4U;
			}

			//shape
			void Line4::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double v[] = {1, r, r * r, r * r * r};
				//shape
				for(uint32_t i = 0; i < 4; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 4; j++)
					{
						N[i] += A[i + 4 * j] / 16 * v[j];
					}
				}
			}
			void Line4::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double dv[] = {0, 1, 2 * r, 3 * r * r};
				//shape
				for(uint32_t i = 0; i < 4; i++)
				{
					B[i] = 0;
					for(uint32_t j = 0; j < 4; j++)
					{
						B[i] += A[i + 4 * j] / 16 * dv[j];
					}
				}
			}
		}
	}
}
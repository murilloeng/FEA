//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri10.hpp"

//static
static const double A[] = {
	 +0,  -1,  -1,  +0,  +0,  +9,  +9,  +0,  +0,   +0,
	 -8,  -1,  +0, +18,  -9, +36,  +9,  -9, +18,  -54,
	 -8,  +0,  -1, +18,  -9,  +9, +36,  -9, +18,  -54,
	-18,  +9,  +0, +45, -36, +27,  +0,  +0, +27,  -54,
	-36,  +0,  +0, +72, -36, +36, +36, -36, +72, -108, 
	-18,  +0,  +9, +27,  +0,  +0, +27, -36, +45,  -54, 
	 -9,  +9,  +0, +27, -27,  +0,  +0,  +0,  +0,   +0, 
	-27,  +0,  +0, +54, -27, +27,  +0,  +0, +27,  -54, 
	-27,  +0,  +0, +27,  +0,  +0, +27, -27, +54,  -54,
	 -9,  +0,  +9,  +0,  +0,  +0,  +0, -27, +27,   +0
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri10::Tri10(void) : Tri(3)
			{
				return;
			}

			//destructor
			Tri10::~Tri10(void)
			{
				return;
			}

			//geometry
			uint32_t Tri10::vertices(void) const
			{
				return 10U;
			}

			//integration
			void Tri10::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double v[] = {1, r, s, r * r, r * s, s * s, r * r * r, r * r * s, r * s * s, s * s * s};
				//shape
				for(uint32_t i = 0; i < 10; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 10; j++)
					{
						N[i] += A[i + 10 * j] / 16 * v[j];
					}
				}
			}
			void Tri10::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double dv[] = {
					0, 1, 0, 2 * r, s, 0, 3 * r * r, 2 * r * s, s * s, 0,
					0, 0, 1, 0, r, 2 * s, 0, r * r, 2 * r * s, 3 * s * s
				};
				//gradient
				for(uint32_t i = 0; i < 10; i++)
				{
					B[i +  0] = B[i + 10] = 0;
					for(uint32_t j = 0; j < 10; j++)
					{
						B[i +  0] += A[i + 10 * j] / 16 * dv[j +  0];
						B[i + 10] += A[i + 10 * j] / 16 * dv[j + 10];
					}
				}
			}
		}
	}
}
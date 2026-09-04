//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri9.hpp"

//static
static const double A[] = {
	 +0,  -1,  -1,  +0,  +0,  +9,  +9,  +0,  +0, 
	-14,  -7,  -6, +12, -15, +30,  +3, -15, +12, 
	-14,  -6,  -7, +12, -15,  +3, +30, -15, +12, 
	-24,  +3,  -6, +39, -42, +21,  -6,  -6, +21, 
	-48, -12, -12, +60, -48, +24, +24, -48, +60, 
	-24,  -6,  +3, +21,  -6,  -6, +21, -42, +39, 
	 -9,  +9,  +0, +27, -27,  +0,  +0,  +0,  +0, 
	-33,  -6,  -6, +48, -33, +21,  -6,  -6, +21, 
	-33,  -6,  -6, +21,  -6,  -6, +21, -33, +48, 
	 -9,  +0,  +9,  +0,  +0,  +0,  +0, -27, +27
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri9::Tri9(void) : Tri(3)
			{
				return;
			}

			//destructor
			Tri9::~Tri9(void)
			{
				return;
			}

			//geometry
			uint32_t Tri9::vertices(void) const
			{
				return 9U;
			}

			//integration
			void Tri9::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double v[] = {1, r, s, r * r, r * s, s * s, r * r * r, r * r * s, r * s * s, s * s * s};
				//shape
				for(uint32_t i = 0; i < 9; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 10; j++)
					{
						N[i] += A[i + 9 * j] / 16 * v[j];
					}
				}
			}
			void Tri9::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double dv[] = {
					0, 1, 0, 2 * r, s, 0, 3 * r * r, 2 * r * s, s * s, 0,
					0, 0, 1, 0, r, 2 * s, 0, r * r, 2 * r * s, 3 * s * s
				};
				//gradient
				for(uint32_t i = 0; i < 9; i++)
				{
					B[i + 0] = B[i + 9] = 0;
					for(uint32_t j = 0; j < 10; j++)
					{
						B[i + 0] += A[i + 9 * j] / 16 * dv[j + 0];
						B[i + 9] += A[i + 9 * j] / 16 * dv[j + 9];
					}
				}
			}
		}
	}
}
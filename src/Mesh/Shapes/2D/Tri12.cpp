//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri12.hpp"

//static
static const double A[] = {
	 +0, +0, +0,  +0,  +0,  +0,  +0,  +6,  +0,  +0,  +0,  +0, 
	 +1, -3, -2, -10,  +4,  -2,  +6, +16,  -2,  -2,  +4, -10, 
	 +1, -2, -3, -10,  +4,  -2,  -2, +16,  +6,  -2,  +4, -10, 
	 +9, -3, -2, -34, +28, -10, +22, +10,  -2,  -2, +10, -26, 
	+18, -4, -4, -60, +38, -12,  +4, +50,  +4, -12, +38, -60, 
	 +9, -2, -3, -26, +10,  -2,  -2, +10, +22, -10, +28, -34, 
	+12, +4, +0, -40, +48, -24, +16,  +0,  +0,  +0,  +0, -16, 
	+34, -2, -2, -98, +82, -26, +22, +34,  -2,  -2, +34, -74, 
	+34, -2, -2, -74, +34,  -2,  -2, +34, +22, -26, +82, -98, 
	+12, +0, +4, -16,  +0,  +0,  +0,  +0, +16, -24, +48, -40, 
	 +4, +4, +0, -16, +24, -16,  +0,  +0,  +0,  +0,  +0,  +0, 
	+16, +0, +0, -48, +48, -16, +16,  +0,  +0,  +0,  +0, -16, 
	+24, +0, +0, -48, +24,  +0,  +0, +24,  +0,  +0, +24, -48, 
	+16, +0, +0, -16,  +0,  +0,  +0,  +0, +16, -16, +48, -48, 
	 +4, +0, +4,  +0,  +0,  +0,  +0,  +0,  +0, -16, +24, -16
};

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri12::Tri12(void) : Tri(3)
			{
				return;
			}

			//destructor
			Tri12::~Tri12(void)
			{
				return;
			}

			//geometry
			uint32_t Tri12::vertices(void) const
			{
				return 12U;
			}

			//integration
			void Tri12::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double v[] = {
					1, r, s, r * r, r * s, s * s, r * r * r, r * r * s, r * s * s, s * s * s, 
					r * r * r * r, r * r * r * s, r * r * s * s, r * s * s * s, s * s * s * s
				};
				//shape
				for(uint32_t i = 0; i < 12; i++)
				{
					N[i] = 0;
					for(uint32_t j = 0; j < 15; j++)
					{
						N[i] += A[i + 12 * j] / 6 * v[j];
					}
				}
			}
			void Tri12::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double dv[] = {
					0, 1, 0, 2 * r, s, 0, 3 * r * r, 2 * r * s, s * s, 0, 4 * r * r * r, 3 * r * r * s, 2 * r * s * s, s * s * s, 0,
					0, 0, 1, 0, r, 2 * s, 0, r * r, 2 * r * s, 3 * s * s, 0, r * r * r, 2 * r * r * s, 3 * r * s * s, 4 * s * s * s
				};
				//gradient
				for(uint32_t i = 0; i < 12; i++)
				{
					B[i +  0] = B[i + 12] = 0;
					for(uint32_t j = 0; j < 15; j++)
					{
						B[i +  0] += A[i + 12 * j] / 6 * dv[j +  0];
						B[i + 12] += A[i + 12 * j] / 6 * dv[j + 12];
					}
				}
			}
		}
	}
}
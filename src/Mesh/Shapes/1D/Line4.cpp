//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line4.hpp"

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
			void Line4::function(double* N, const double* s) const
			{
				N[0] = (-1 + s[0] + 9 * s[0] * s[0] - 9 * s[0] * s[0] * s[0]) / 16;
				N[3] = (-1 - s[0] + 9 * s[0] * s[0] + 9 * s[0] * s[0] * s[0]) / 16;
				N[1] = 9 * (1 - 3 * s[0] - s[0] * s[0] + 3 * s[0] * s[0] * s[0]) / 16;
				N[2] = 9 * (1 + 3 * s[0] - s[0] * s[0] - 3 * s[0] * s[0] * s[0]) / 16;
			}
			void Line4::gradient(double* B, const double* s) const
			{
				B[0] = (+1 + 18 * s[0] - 27 * s[0] * s[0]) / 16;
				B[3] = (-1 + 18 * s[0] + 27 * s[0] * s[0]) / 16;
				B[1] = 9 * (-3 - 2 * s[0] + 9 * s[0] * s[0]) / 16;
				B[2] = 9 * (+3 - 2 * s[0] - 9 * s[0] * s[0]) / 16;
			}
		}
	}
}
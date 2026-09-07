//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line5.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line5::Line5(void) : Line(2)
			{
				return;
			}

			//destructor
			Line5::~Line5(void)
			{
				return;
			}

			//geometry
			uint32_t Line5::vertices(void) const
			{
				return 2U;
			}

			//shape
			void Line5::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				//shape
				N[0] = +r * (1 - r) * (1 - 2 * r) * (1 + 2 * r) / 6;
				N[1] = -r * (1 + r) * (1 - 2 * r) * (1 + 2 * r) / 6;
				N[2] = -4 * r * (1 - r) * (1 + r) * (1 - 2 * r) / 3;
				N[4] = +4 * r * (1 - r) * (1 + r) * (1 + 2 * r) / 3;
				N[3] = +(1 - r) * (1 + r) * (1 - 2 * r) * (1 + 2 * r);
			}
			void Line5::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				//gradient
				B[3] = 2 * r * (8 * r * r - 5);
				B[0] = (4 * r - 1) * (4 * r * r - 2 * r - 1) / 6;
				B[1] = (4 * r + 1) * (4 * r * r + 2 * r - 1) / 6;
				B[2] = -4 * (8 * r * r * r - 3 * r * r - 4 * r + 1) / 3;
				B[4] = -4 * (8 * r * r * r + 3 * r * r - 4 * r - 1) / 3;
			}
		}
	}
}
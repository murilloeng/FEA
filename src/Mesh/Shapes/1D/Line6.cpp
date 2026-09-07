//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line6.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line6::Line6(void) : Line(2)
			{
				return;
			}

			//destructor
			Line6::~Line6(void)
			{
				return;
			}

			//geometry
			uint32_t Line6::vertices(void) const
			{
				return 2U;
			}

			//shape
			void Line6::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				//shape
				N[2] = +25*(r - 1) * (r + 1) * (5 * r - 3) * (5 * r - 1) * (5 * r + 1) / 768;
				N[3] = -25*(r - 1) * (r + 1) * (5 * r - 3) * (5 * r - 1) * (5 * r + 3) / 384;
				N[4] = +25*(r - 1) * (r + 1) * (5 * r - 3) * (5 * r + 1) * (5 * r + 3) / 384;
				N[5] = -25*(r - 1) * (r + 1) * (5 * r - 1) * (5 * r + 1) * (5 * r + 3) / 768;
				N[0] = -(r - 1) * (5 * r - 3) * (5 * r - 1) * (5 * r + 1) * (5 * r + 3) / 768;
				N[1] = +(r + 1) * (5 * r - 3) * (5 * r - 1) * (5 * r + 1) * (5 * r + 3) / 768;
			}
			void Line6::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				//gradient
				B[0] = -(3125 * r * r * r * r - 2500 * r * r * r - 750 * r * r + 500 * r + 9) / 768;
				B[1] = +(3125 * r * r * r * r + 2500 * r * r * r - 750 * r * r - 500 * r + 9) / 768;
				B[2] = +25*(625 * r * r * r * r - 300 * r * r * r - 390 * r * r + 156 * r + 5) / 768;
				B[3] = -25*(625 * r * r * r * r - 100 * r * r * r - 510 * r * r + 68 * r + 45) / 384;
				B[4] = +25*(625 * r * r * r * r + 100 * r * r * r - 510 * r * r - 68 * r + 45) / 384;
				B[5] = -25*(625 * r * r * r * r + 300 * r * r * r - 390 * r * r - 156 * r + 5) / 768;
			}
		}
	}
}
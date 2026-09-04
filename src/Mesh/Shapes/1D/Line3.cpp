//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line3.hpp"

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
				//shape
				N[2] = 1 - r * r;
				N[0] = r * (r - 1) / 2;
				N[1] = r * (r + 1) / 2;
			}
			void Line3::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				//gradient
				B[2] = -2 * r;
				B[0] = r - 0.5;
				B[1] = r + 0.5;
			}
		}
	}
}
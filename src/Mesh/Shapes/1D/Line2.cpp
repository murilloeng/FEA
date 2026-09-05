//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line2.hpp"

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
				//shape
				N[0] = (1 - r) / 2;
				N[1] = (1 + r) / 2;
			}
			void Line2::gradient(double* B, const double* p) const
			{
				//gradient
				B[0] = -1.0 / 2;
				B[1] = +1.0 / 2;
			}
		}
	}
}
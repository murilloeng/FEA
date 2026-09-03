//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line2.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line2::Line2(void)
			{
				return;
			}
			
			//destructor
			Line2::~Line2(void)
			{
				return;
			}

			//shape
			void Line2::function(double* N, const double* s) const
			{
				N[0] = (1 - s[0]) / 2;
				N[1] = (1 + s[0]) / 2;
			}
			void Line2::gradient(double* B, const double* s) const
			{
				B[0] = -0.5;
				B[1] = +0.5;
			}
		}
	}
}
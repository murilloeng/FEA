//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line3.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line3::Line3(void)
			{
				return;
			}
			
			//destructor
			Line3::~Line3(void)
			{
				return;
			}

			//shape
			void Line3::function(double* N, const double* s) const
			{
				N[1] = 1 - s[0] * s[0];
				N[0] = (s[0] * s[0] - s[0]) / 2;
				N[2] = (s[0] * s[0] + s[0]) / 2;
			}
			void Line3::gradient(double* B, const double* s) const
			{
				B[1] = -2 * s[0];
				B[0] = s[0] - 0.5;
				B[2] = s[0] + 0.5;
			}
		}
	}
}
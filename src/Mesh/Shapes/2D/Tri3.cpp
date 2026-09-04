//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri3.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri3::Tri3(void) : Tri(1)
			{
				return;
			}

			//destructor
			Tri3::~Tri3(void)
			{
				return;
			}

			//geometry
			uint32_t Tri3::vertices(void) const
			{
				return 3U;
			}

			//integration
			void Tri3::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[0] = -(r + s) / 2;
				N[1] = +(1 + r) / 2;
				N[2] = +(1 + s) / 2;
			}
			void Tri3::gradient(double* B, const double* p) const
			{
				//gradient r
				B[0 + 0] = -0.5;
				B[1 + 0] = +0.5;
				B[2 + 0] = +0.0;
				//gradient s
				B[0 + 3] = -0.5;
				B[1 + 3] = +0.0;
				B[2 + 3] = +0.5;
			}
		}
	}
}
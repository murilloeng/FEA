//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri6.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri6::Tri6(void) : Tri(2)
			{
				return;
			}

			//destructor
			Tri6::~Tri6(void)
			{
				return;
			}

			//geometry
			uint32_t Tri6::vertices(void) const
			{
				return 6U;
			}

			//integration
			void Tri6::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[1] = r * (1 + r) / 2;
				N[2] = s * (1 + s) / 2;
				N[3] = -(1 + r) * (r + s);
				N[4] = +(1 + r) * (1 + s);
				N[5] = -(1 + s) * (r + s);
				N[0] = +(r + s) * (1 + r + s) / 2;
			}
			void Tri6::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//gradient r
				B[2 + 0] = 0;
				B[4 + 0] = +(1 + s);
				B[5 + 0] = -(1 + s);
				B[1 + 0] = +(1 + 2 * r) / 2;
				B[3 + 0] = -(1 + 2 * r + s);
				B[0 + 0] = +(1 + 2 * r + 2 * s) / 2;
				//gradient s
				B[1 + 6] = 0;
				B[3 + 6] = -(1 + r);
				B[4 + 6] = +(1 + r);
				B[2 + 6] = +(1 + 2 * s) / 2;
				B[5 + 6] = -(1 + r + 2 * s);
				B[0 + 6] = +(1 + 2 * r + 2 * s) / 2;
			}
		}
	}
}
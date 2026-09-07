//FEA
#include "FEA/inc/Mesh/Shapes/2D/Quadrangle4.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Quadrangle4::Quadrangle4(void) : Quadrangle(2)
			{
				return;
			}

			//destructor
			Quadrangle4::~Quadrangle4(void)
			{
				return;
			}

			//geometry
			uint32_t Quadrangle4::vertices(void) const
			{
				return 4U;
			}

			//integration
			void Quadrangle4::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[0] = (1 - r) * (1 - s) / 4;
				N[1] = (1 + r) * (1 - s) / 4;
				N[2] = (1 + r) * (1 + s) / 4;
				N[3] = (1 - r) * (1 + s) / 4;
			}
			void Quadrangle4::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//gradient r
				B[0 + 0] = -(1 - s) / 4;
				B[1 + 0] = +(1 - s) / 4;
				B[2 + 0] = +(1 + s) / 4;
				B[3 + 0] = -(1 + s) / 4;
				//gradient s
				B[0 + 4] = -(1 - r) / 4;
				B[1 + 4] = -(1 + r) / 4;
				B[2 + 4] = +(1 + r) / 4;
				B[3 + 4] = +(1 - r) / 4;
			}
		}
	}
}
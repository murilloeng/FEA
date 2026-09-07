//FEA
#include "FEA/inc/Mesh/Shapes/3D/Hexahedron8.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Hexahedron8::Hexahedron8(void) : Hexahedron(1)
			{
				return;
			}

			//destructor
			Hexahedron8::~Hexahedron8(void)
			{
				return;
			}

			//geometry
			uint32_t Hexahedron8::vertices(void) const
			{
				return 8U;
			}

			//integration
			void Hexahedron8::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				//shape
				N[0] = +(1 - r) * (1 - s) * (1 - t) / 8;
				N[1] = +(1 + r) * (1 - s) * (1 - t) / 8;
				N[2] = +(1 + r) * (1 + s) * (1 - t) / 8;
				N[3] = +(1 - r) * (1 + s) * (1 - t) / 8;
				N[4] = +(1 - r) * (1 - s) * (1 + t) / 8;
				N[5] = +(1 + r) * (1 - s) * (1 + t) / 8;
				N[6] = +(1 + r) * (1 + s) * (1 + t) / 8;
				N[7] = +(1 - r) * (1 + s) * (1 + t) / 8;
			}
			void Hexahedron8::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				//gradient r
				B[0 +  0] = -(1 - s) * (1 - t) / 8;
				B[1 +  0] = +(1 - s) * (1 - t) / 8;
				B[2 +  0] = +(1 + s) * (1 - t) / 8;
				B[3 +  0] = -(1 + s) * (1 - t) / 8;
				B[4 +  0] = -(1 - s) * (1 + t) / 8;
				B[5 +  0] = +(1 - s) * (1 + t) / 8;
				B[6 +  0] = +(1 + s) * (1 + t) / 8;
				B[7 +  0] = -(1 + s) * (1 + t) / 8;
				//gradient s
				B[0 +  8] = -(1 - r) * (1 - t) / 8;
				B[1 +  8] = -(1 + r) * (1 - t) / 8;
				B[2 +  8] = +(1 + r) * (1 - t) / 8;
				B[3 +  8] = +(1 - r) * (1 - t) / 8;
				B[4 +  8] = -(1 - r) * (1 + t) / 8;
				B[5 +  8] = -(1 + r) * (1 + t) / 8;
				B[6 +  8] = +(1 + r) * (1 + t) / 8;
				B[7 +  8] = +(1 - r) * (1 + t) / 8;
				//gradient t
				B[0 + 16] = -(1 - r) * (1 - s) / 8;
				B[1 + 16] = -(1 + r) * (1 - s) / 8;
				B[2 + 16] = -(1 + r) * (1 + s) / 8;
				B[3 + 16] = -(1 - r) * (1 + s) / 8;
				B[4 + 16] = +(1 - r) * (1 - s) / 8;
				B[5 + 16] = +(1 + r) * (1 - s) / 8;
				B[6 + 16] = +(1 + r) * (1 + s) / 8;
				B[7 + 16] = +(1 - r) * (1 + s) / 8;
			}
		}
	}
}
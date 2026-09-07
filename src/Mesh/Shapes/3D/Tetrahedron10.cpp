//FEA
#include "FEA/inc/Mesh/Shapes/3D/Tetrahedron10.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tetrahedron10::Tetrahedron10(void) : Tetrahedron(1)
			{
				return;
			}

			//destructor
			Tetrahedron10::~Tetrahedron10(void)
			{
				return;
			}

			//geometry
			uint32_t Tetrahedron10::vertices(void) const
			{
				return 10U;
			}

			//integration
			void Tetrahedron10::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				//shape
				N[1] = +r * (1 + r) / 2;
				N[2] = +s * (1 + s) / 2;
				N[3] = +t * (1 + t) / 2;
				N[5] = +(1 + r) * (1 + s);
				N[8] = +(1 + r) * (1 + t);
				N[9] = +(1 + s) * (1 + t);
				N[4] = -(1 + r) * (1 + r + s + t);
				N[6] = -(1 + s) * (1 + r + s + t);
				N[7] = -(1 + t) * (1 + r + s + t);
				N[0] = +(1 + r + s + t) * (2 + r + s + t) / 2;
			}
			void Tetrahedron10::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				//gradient r
				B[2 +  0] = 0;
				B[3 +  0] = 0;
				B[9 +  0] = 0;
				B[5 +  0] = +(1 + s);
				B[6 +  0] = -(1 + s);
				B[7 +  0] = -(1 + t);
				B[8 +  0] = +(1 + t);
				B[1 +  0] = +(1 + 2 * r) / 2;
				B[4 +  0] = -(2 + 2 * r + s + t);
				B[0 +  0] = +(3 + 2 * r + 2 * s + 2 * t) / 2;
				//gradient s
				B[1 + 10] = 0;
				B[3 + 10] = 0;
				B[8 + 10] = 0;
				B[4 + 10] = -(1 + r);
				B[5 + 10] = +(1 + r);
				B[7 + 10] = -(1 + t);
				B[9 + 10] = +(1 + t);
				B[2 + 10] = +(1 + 2 * s) / 2;
				B[6 + 10] = -(2 + r + 2 * s + t);
				B[0 + 10] = +(3 + 2 * r + 2 * s + 2 * t) / 2;
				//gradient t
				B[1 + 20] = 0;
				B[2 + 20] = 0;
				B[5 + 20] = 0;
				B[4 + 20] = -(1 + r);
				B[6 + 20] = -(1 + s);
				B[8 + 20] = +(1 + r);
				B[9 + 20] = +(1 + s);
				B[3 + 20] = +(1 + 2 * t) / 2;
				B[7 + 20] = -(2 + r + s + 2 * t);
				B[0 + 20] = +(3 + 2 * r + 2 * s + 2 * t) / 2;
			}
		}
	}
}
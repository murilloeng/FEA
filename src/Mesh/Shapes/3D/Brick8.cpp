//FEA
#include "FEA/inc/Mesh/Shapes/3D/Brick8.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Brick8::Brick8(void) : Brick(1)
			{
				return;
			}

			//destructor
			Brick8::~Brick8(void)
			{
				return;
			}

			//geometry
			uint32_t Brick8::vertices(void) const
			{
				return 8U;
			}

			//integration
			void Brick8::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				//shape
				N[0] = -(r - 1) * (s - 1) * (t - 1) / 8;
				N[1] = +(r + 1) * (s - 1) * (t - 1) / 8;
				N[2] = -(r + 1) * (s + 1) * (t - 1) / 8;
				N[3] = +(r - 1) * (s + 1) * (t - 1) / 8;
				N[4] = +(r - 1) * (s - 1) * (t + 1) / 8;
				N[5] = -(r + 1) * (s - 1) * (t + 1) / 8;
				N[6] = +(r + 1) * (s + 1) * (t + 1) / 8;
				N[7] = -(r - 1) * (s + 1) * (t + 1) / 8;
			}
			void Brick8::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				//gradient r
				B[0 +  0] = -(s - 1) * (t - 1) / 8;
				B[1 +  0] = +(s - 1) * (t - 1) / 8;
				B[2 +  0] = -(s + 1) * (t - 1) / 8;
				B[3 +  0] = +(s + 1) * (t - 1) / 8;
				B[4 +  0] = +(s - 1) * (t + 1) / 8;
				B[5 +  0] = -(s - 1) * (t + 1) / 8;
				B[6 +  0] = +(s + 1) * (t + 1) / 8;
				B[7 +  0] = -(s + 1) * (t + 1) / 8;
				//gradient s
				B[0 +  8] = -(r - 1) * (t - 1) / 8;
				B[1 +  8] = +(r + 1) * (t - 1) / 8;
				B[2 +  8] = -(r + 1) * (t - 1) / 8;
				B[3 +  8] = +(r - 1) * (t - 1) / 8;
				B[4 +  8] = +(r - 1) * (t + 1) / 8;
				B[5 +  8] = -(r + 1) * (t + 1) / 8;
				B[6 +  8] = +(r + 1) * (t + 1) / 8;
				B[7 +  8] = -(r - 1) * (t + 1) / 8;
				//gradient t
				B[0 + 16] = -(r - 1) * (t - 1) / 8;
				B[1 + 16] = +(r + 1) * (t - 1) / 8;
				B[2 + 16] = -(r + 1) * (t - 1) / 8;
				B[3 + 16] = +(r - 1) * (t - 1) / 8;
				B[4 + 16] = +(r - 1) * (t + 1) / 8;
				B[5 + 16] = -(r + 1) * (t + 1) / 8;
				B[6 + 16] = +(r + 1) * (t + 1) / 8;
				B[7 + 16] = -(r - 1) * (t + 1) / 8;
			}
		}
	}
}
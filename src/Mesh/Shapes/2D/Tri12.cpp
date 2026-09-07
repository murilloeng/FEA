//FEA
#include "FEA/inc/Mesh/Shapes/2D/Tri12.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tri12::Tri12(void) : Tri(3)
			{
				return;
			}

			//destructor
			Tri12::~Tri12(void)
			{
				return;
			}

			//geometry
			uint32_t Tri12::vertices(void) const
			{
				return 12U;
			}

			//integration
			void Tri12::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//shape
				N[ 6] = +(r + 1) * (s + 1) * (8 * r * r + 3 * r - s) / 3;
				N[ 8] = -(r + 1) * (s + 1) * (r - 8 * s * s - 3 * s) / 3;
				N[ 5] = -(r + 1) * (r + s) * (8 * r * r + 4 * r + s + 1) / 3;
				N[ 9] = -(r + s) * (s + 1) * (r + 8 * s * s + 4 * s + 1) / 3;
				N[ 7] = +(r + 1) * (s + 1) * (12 * r * s + 5 * r + 5 * s + 3) / 3;
				N[ 1] = +(r + 1) * (4 * r * r * r - 2 * r * s - 3 * r - 2 * s * s - 2 * s) / 6;
				N[ 2] = -(s + 1) * (2 * r * r + 2 * r * s + 2 * r - 4 * s * s * s + 3 * s) / 6;
				N[10] = +(r + s) * (s + 1) * (12 * r * s + 5 * r + 12 * s * s + 12 * s + 2) / 3;
				N[ 4] = +(r + 1) * (r + s) * (12 * r * r + 12 * r * s + 12 * r + 5 * s + 2) / 3;
				N[ 3] = -(r + 1) * (r + s) * (8 * r * r + 16 * r * s + 12 * r + 8 * s * s + 13 * s + 5) / 3;
				N[11] = -(r + s) * (s + 1) * (8 * r * r + 16 * r * s + 13 * r + 8 * s * s + 12 * s + 5) / 3;
				N[ 0] = +(r + s) * (4 * r * r * r + 12 * r * r * s + 12 * r * r + 12 * r * s * s + 22 * r * s + 9 * r + 4 * s * s * s + 12 * s * s + 9 * s + 1) / 6;
			}
			void Tri12::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				//gradient r
				B[ 2 +  0] = -(s + 1) * (2 * r + s + 1) / 3;
				B[ 6 +  0] = +(s + 1) * (24 * r * r + 22 * r - s + 3) / 3;
				B[ 8 +  0] = -(s + 1) * (2 * r - 8 * s * s - 3 * s + 1) / 3;
				B[ 9 +  0] = -(s + 1) * (2 * r + 8 * s * s + 5 * s + 1) / 3;
				B[ 7 +  0] = +(s + 1) * (24 * r * s + 10 * r + 17 * s + 8) / 3;
				B[10 +  0] = +(s + 1) * (24 * r * s + 10 * r + 24 * s * s + 17 * s + 2) / 3;
				B[ 4 +  0] = +(2 * r + s + 1) * (24 * r * r + 24 * r * s + 24 * r + 17 * s + 2) / 3;
				B[11 +  0] = -(s + 1) * (24 * r * r + 48 * r * s + 26 * r + 24 * s * s + 25 * s + 5) / 3;
				B[ 1 +  0] = (16 * r * r * r + 12 * r * r - 4 * r * s - 6 * r - 2 * s * s - 4 * s - 3) / 6;
				B[ 5 +  0] = -(32 * r * r * r + 24 * r * r * s + 36 * r * r + 26 * r * s + 10 * r + s * s + 6 * s + 1) / 3;
				B[ 0 +  0] = (16 * r * r * r + 48 * r * r * s + 36 * r * r + 48 * r * s * s + 68 * r * s + 18 * r + 16 * s * s * s + 34 * s * s + 18 * s + 1) / 6;
				B[ 3 +  0] = -(32 * r * r * r + 72 * r * r * s + 60 * r * r + 48 * r * s * s + 98 * r * s + 34 * r + 8 * s * s * s + 37 * s * s + 30 * s + 5) / 3;
				//gradient s
				B[ 1 + 12] = -(r + 1) * (r + 2 * s + 1) / 3;
				B[ 8 + 12] = -(r + 1) * (r - 24 * s * s - 22 * s - 3) / 3;
				B[ 5 + 12] = -(r + 1) * (8 * r * r + 5 * r + 2 * s + 1) / 3;
				B[ 6 + 12] = +(r + 1) * (8 * r * r + 3 * r - 2 * s - 1) / 3;
				B[ 7 + 12] = +(r + 1) * (24 * r * s + 17 * r + 10 * s + 8) / 3;
				B[ 4 + 12] = +(r + 1) * (24 * r * r + 24 * r * s + 17 * r + 10 * s + 2) / 3;
				B[10 + 12] = +(r + 2 * s + 1) * (24 * r * s + 17 * r + 24 * s * s + 24 * s + 2) / 3;
				B[ 3 + 12] = -(r + 1) * (24 * r * r + 48 * r * s + 25 * r + 24 * s * s + 26 * s + 5) / 3;
				B[ 2 + 12] = -(2 * r * r + 4 * r * s + 4 * r - 16 * s * s * s - 12 * s * s + 6 * s + 3) / 6;
				B[ 9 + 12] = -(r * r + 24 * r * s * s + 26 * r * s + 6 * r + 32 * s * s * s + 36 * s * s + 10 * s + 1) / 3;
				B[ 0 + 12] = (16 * r * r * r + 48 * r * r * s + 34 * r * r + 48 * r * s * s + 68 * r * s + 18 * r + 16 * s * s * s + 36 * s * s + 18 * s + 1) / 6;
				B[11 + 12] = -(8 * r * r * r + 48 * r * r * s + 37 * r * r + 72 * r * s * s + 98 * r * s + 30 * r + 32 * s * s * s + 60 * s * s + 34 * s + 5) / 3;
			}
		}
	}
}
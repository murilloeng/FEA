//FEA
#include "FEA/inc/Mesh/Shapes/3D/Hexahedron20.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Hexahedron20::Hexahedron20(void) : Hexahedron(1)
			{
				return;
			}

			//destructor
			Hexahedron20::~Hexahedron20(void)
			{
				return;
			}

			//geometry
			uint32_t Hexahedron20::vertices(void) const
			{
				return 20U;
			}

			//integration
			void Hexahedron20::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				//shape
				N[ 8] = -(3 * r * r * s * s * t * t - 5 * r * r * s * s * t + r * r * s * s - 5 * r * r * s * t * t + 5 * r * r * s * t + r * r * t * t - 4 * s * s * t * t + 5 * s * s * t + 5 * s * t * t - 5 * s * t - 1) / 20;
				N[ 9] = -(3 * r * r * s * s * t * t - 5 * r * r * s * s * t + r * r * s * s - 4 * r * r * t * t + 5 * r * r * t - 5 * r * s * s * t * t + 5 * r * s * s * t + 5 * r * t * t - 5 * r * t + s * s * t * t - 1) / 20;
				N[10] = -(3 * r * r * s * s * t * t - 4 * r * r * s * s - 5 * r * r * s * t * t + 5 * r * r * s + r * r * t * t - 5 * r * s * s * t * t + 5 * r * s * s + 5 * r * s * t * t - 5 * r * s + s * s * t * t - 1) / 20;
				N[11] = -(3 * r * r * s * s * t * t - 5 * r * r * s * s * t + r * r * s * s - 4 * r * r * t * t + 5 * r * r * t + 5 * r * s * s * t * t - 5 * r * s * s * t - 5 * r * t * t + 5 * r * t + s * s * t * t - 1) / 20;
				N[12] = -(3 * r * r * s * s * t * t - 4 * r * r * s * s - 5 * r * r * s * t * t + 5 * r * r * s + r * r * t * t + 5 * r * s * s * t * t - 5 * r * s * s - 5 * r * s * t * t + 5 * r * s + s * s * t * t - 1) / 20;
				N[13] = -(3 * r * r * s * s * t * t - 5 * r * r * s * s * t + r * r * s * s + 5 * r * r * s * t * t - 5 * r * r * s * t + r * r * t * t - 4 * s * s * t * t + 5 * s * s * t - 5 * s * t * t + 5 * s * t - 1) / 20;
				N[14] = -(3 * r * r * s * s * t * t - 4 * r * r * s * s + 5 * r * r * s * t * t - 5 * r * r * s + r * r * t * t + 5 * r * s * s * t * t - 5 * r * s * s + 5 * r * s * t * t - 5 * r * s + s * s * t * t - 1) / 20;
				N[15] = -(3 * r * r * s * s * t * t - 4 * r * r * s * s + 5 * r * r * s * t * t - 5 * r * r * s + r * r * t * t - 5 * r * s * s * t * t + 5 * r * s * s - 5 * r * s * t * t + 5 * r * s + s * s * t * t - 1) / 20;
				N[16] = -(3 * r * r * s * s * t * t + 5 * r * r * s * s * t + r * r * s * s - 5 * r * r * s * t * t - 5 * r * r * s * t + r * r * t * t - 4 * s * s * t * t - 5 * s * s * t + 5 * s * t * t + 5 * s * t - 1) / 20;
				N[17] = -(3 * r * r * s * s * t * t + 5 * r * r * s * s * t + r * r * s * s - 4 * r * r * t * t - 5 * r * r * t - 5 * r * s * s * t * t - 5 * r * s * s * t + 5 * r * t * t + 5 * r * t + s * s * t * t - 1) / 20;
				N[18] = -(3 * r * r * s * s * t * t + 5 * r * r * s * s * t + r * r * s * s - 4 * r * r * t * t - 5 * r * r * t + 5 * r * s * s * t * t + 5 * r * s * s * t - 5 * r * t * t - 5 * r * t + s * s * t * t - 1) / 20;
				N[19] = -(3 * r * r * s * s * t * t + 5 * r * r * s * s * t + r * r * s * s + 5 * r * r * s * t * t + 5 * r * r * s * t + r * r * t * t - 4 * s * s * t * t - 5 * s * s * t - 5 * s * t * t - 5 * s * t - 1) / 20;
				N[ 0] = (9 * r * r * s * s * t * t - 5 * r * r * s * s * t - 2 * r * r * s * s - 5 * r * r * s * t * t + 5 * r * r * s * t - 2 * r * r * t * t - 5 * r * s * s * t * t + 5 * r * s * s * t + 5 * r * s * t * t - 5 * r * s * t - 2 * s * s * t * t + 2) / 40;
				N[ 1] = (9 * r * r * s * s * t * t - 5 * r * r * s * s * t - 2 * r * r * s * s - 5 * r * r * s * t * t + 5 * r * r * s * t - 2 * r * r * t * t + 5 * r * s * s * t * t - 5 * r * s * s * t - 5 * r * s * t * t + 5 * r * s * t - 2 * s * s * t * t + 2) / 40;
				N[ 2] = (9 * r * r * s * s * t * t - 5 * r * r * s * s * t - 2 * r * r * s * s + 5 * r * r * s * t * t - 5 * r * r * s * t - 2 * r * r * t * t + 5 * r * s * s * t * t - 5 * r * s * s * t + 5 * r * s * t * t - 5 * r * s * t - 2 * s * s * t * t + 2) / 40;
				N[ 3] = (9 * r * r * s * s * t * t - 5 * r * r * s * s * t - 2 * r * r * s * s + 5 * r * r * s * t * t - 5 * r * r * s * t - 2 * r * r * t * t - 5 * r * s * s * t * t + 5 * r * s * s * t - 5 * r * s * t * t + 5 * r * s * t - 2 * s * s * t * t + 2) / 40;
				N[ 4] = (9 * r * r * s * s * t * t + 5 * r * r * s * s * t - 2 * r * r * s * s - 5 * r * r * s * t * t - 5 * r * r * s * t - 2 * r * r * t * t - 5 * r * s * s * t * t - 5 * r * s * s * t + 5 * r * s * t * t + 5 * r * s * t - 2 * s * s * t * t + 2) / 40;
				N[ 5] = (9 * r * r * s * s * t * t + 5 * r * r * s * s * t - 2 * r * r * s * s - 5 * r * r * s * t * t - 5 * r * r * s * t - 2 * r * r * t * t + 5 * r * s * s * t * t + 5 * r * s * s * t - 5 * r * s * t * t - 5 * r * s * t - 2 * s * s * t * t + 2) / 40;
				N[ 6] = (9 * r * r * s * s * t * t + 5 * r * r * s * s * t - 2 * r * r * s * s + 5 * r * r * s * t * t + 5 * r * r * s * t - 2 * r * r * t * t + 5 * r * s * s * t * t + 5 * r * s * s * t + 5 * r * s * t * t + 5 * r * s * t - 2 * s * s * t * t + 2) / 40;
				N[ 7] = (9 * r * r * s * s * t * t + 5 * r * r * s * s * t - 2 * r * r * s * s + 5 * r * r * s * t * t + 5 * r * r * s * t - 2 * r * r * t * t - 5 * r * s * s * t * t - 5 * r * s * s * t - 5 * r * s * t * t - 5 * r * s * t - 2 * s * s * t * t + 2) / 40;
			}
			void Hexahedron20::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				const double s = p[1];
				const double t = p[2];
				//gradient r
				B[ 8 +  0] = -r * (3 * s * s * t * t - 5 * s * s * t + s * s - 5 * s * t * t + 5 * s * t + t * t) / 10;
				B[13 +  0] = -r * (3 * s * s * t * t - 5 * s * s * t + s * s + 5 * s * t * t - 5 * s * t + t * t) / 10;
				B[16 +  0] = -r * (3 * s * s * t * t + 5 * s * s * t + s * s - 5 * s * t * t - 5 * s * t + t * t) / 10;
				B[19 +  0] = -r * (3 * s * s * t * t + 5 * s * s * t + s * s + 5 * s * t * t + 5 * s * t + t * t) / 10;
				B[ 9 +  0] = -(6 * r * s * s * t * t - 10 * r * s * s * t + 2 * r * s * s - 8 * r * t * t + 10 * r * t - 5 * s * s * t * t + 5 * s * s * t + 5 * t * t - 5 * t) / 20;
				B[10 +  0] = -(6 * r * s * s * t * t - 8 * r * s * s - 10 * r * s * t * t + 10 * r * s + 2 * r * t * t - 5 * s * s * t * t + 5 * s * s + 5 * s * t * t - 5 * s) / 20;
				B[11 +  0] = -(6 * r * s * s * t * t - 10 * r * s * s * t + 2 * r * s * s - 8 * r * t * t + 10 * r * t + 5 * s * s * t * t - 5 * s * s * t - 5 * t * t + 5 * t) / 20;
				B[12 +  0] = -(6 * r * s * s * t * t - 8 * r * s * s - 10 * r * s * t * t + 10 * r * s + 2 * r * t * t + 5 * s * s * t * t - 5 * s * s - 5 * s * t * t + 5 * s) / 20;
				B[14 +  0] = -(6 * r * s * s * t * t - 8 * r * s * s + 10 * r * s * t * t - 10 * r * s + 2 * r * t * t + 5 * s * s * t * t - 5 * s * s + 5 * s * t * t - 5 * s) / 20;
				B[15 +  0] = -(6 * r * s * s * t * t - 8 * r * s * s + 10 * r * s * t * t - 10 * r * s + 2 * r * t * t - 5 * s * s * t * t + 5 * s * s - 5 * s * t * t + 5 * s) / 20;
				B[17 +  0] = -(6 * r * s * s * t * t + 10 * r * s * s * t + 2 * r * s * s - 8 * r * t * t - 10 * r * t - 5 * s * s * t * t - 5 * s * s * t + 5 * t * t + 5 * t) / 20;
				B[18 +  0] = -(6 * r * s * s * t * t + 10 * r * s * s * t + 2 * r * s * s - 8 * r * t * t - 10 * r * t + 5 * s * s * t * t + 5 * s * s * t - 5 * t * t - 5 * t) / 20;
				B[ 0 +  0] = (18 * r * s * s * t * t - 10 * r * s * s * t - 4 * r * s * s - 10 * r * s * t * t + 10 * r * s * t - 4 * r * t * t - 5 * s * s * t * t + 5 * s * s * t + 5 * s * t * t - 5 * s * t) / 40;
				B[ 1 +  0] = (18 * r * s * s * t * t - 10 * r * s * s * t - 4 * r * s * s - 10 * r * s * t * t + 10 * r * s * t - 4 * r * t * t + 5 * s * s * t * t - 5 * s * s * t - 5 * s * t * t + 5 * s * t) / 40;
				B[ 2 +  0] = (18 * r * s * s * t * t - 10 * r * s * s * t - 4 * r * s * s + 10 * r * s * t * t - 10 * r * s * t - 4 * r * t * t + 5 * s * s * t * t - 5 * s * s * t + 5 * s * t * t - 5 * s * t) / 40;
				B[ 3 +  0] = (18 * r * s * s * t * t - 10 * r * s * s * t - 4 * r * s * s + 10 * r * s * t * t - 10 * r * s * t - 4 * r * t * t - 5 * s * s * t * t + 5 * s * s * t - 5 * s * t * t + 5 * s * t) / 40;
				B[ 4 +  0] = (18 * r * s * s * t * t + 10 * r * s * s * t - 4 * r * s * s - 10 * r * s * t * t - 10 * r * s * t - 4 * r * t * t - 5 * s * s * t * t - 5 * s * s * t + 5 * s * t * t + 5 * s * t) / 40;
				B[ 5 +  0] = (18 * r * s * s * t * t + 10 * r * s * s * t - 4 * r * s * s - 10 * r * s * t * t - 10 * r * s * t - 4 * r * t * t + 5 * s * s * t * t + 5 * s * s * t - 5 * s * t * t - 5 * s * t) / 40;
				B[ 6 +  0] = (18 * r * s * s * t * t + 10 * r * s * s * t - 4 * r * s * s + 10 * r * s * t * t + 10 * r * s * t - 4 * r * t * t + 5 * s * s * t * t + 5 * s * s * t + 5 * s * t * t + 5 * s * t) / 40;
				B[ 7 +  0] = (18 * r * s * s * t * t + 10 * r * s * s * t - 4 * r * s * s + 10 * r * s * t * t + 10 * r * s * t - 4 * r * t * t - 5 * s * s * t * t - 5 * s * s * t - 5 * s * t * t - 5 * s * t) / 40;
				//gradient s
				B[ 9 + 20] = -s * (3 * r * r * t * t - 5 * r * r * t + r * r - 5 * r * t * t + 5 * r * t + t * t) / 10;
				B[11 + 20] = -s * (3 * r * r * t * t - 5 * r * r * t + r * r + 5 * r * t * t - 5 * r * t + t * t) / 10;
				B[17 + 20] = -s * (3 * r * r * t * t + 5 * r * r * t + r * r - 5 * r * t * t - 5 * r * t + t * t) / 10;
				B[18 + 20] = -s * (3 * r * r * t * t + 5 * r * r * t + r * r + 5 * r * t * t + 5 * r * t + t * t) / 10;
				B[ 8 + 20] = -(6 * r * r * s * t * t - 10 * r * r * s * t + 2 * r * r * s - 5 * r * r * t * t + 5 * r * r * t - 8 * s * t * t + 10 * s * t + 5 * t * t - 5 * t) / 20;
				B[10 + 20] = -(6 * r * r * s * t * t - 8 * r * r * s - 5 * r * r * t * t + 5 * r * r - 10 * r * s * t * t + 10 * r * s + 5 * r * t * t - 5 * r + 2 * s * t * t) / 20;
				B[12 + 20] = -(6 * r * r * s * t * t - 8 * r * r * s - 5 * r * r * t * t + 5 * r * r + 10 * r * s * t * t - 10 * r * s - 5 * r * t * t + 5 * r + 2 * s * t * t) / 20;
				B[13 + 20] = -(6 * r * r * s * t * t - 10 * r * r * s * t + 2 * r * r * s + 5 * r * r * t * t - 5 * r * r * t - 8 * s * t * t + 10 * s * t - 5 * t * t + 5 * t) / 20;
				B[14 + 20] = -(6 * r * r * s * t * t - 8 * r * r * s + 5 * r * r * t * t - 5 * r * r + 10 * r * s * t * t - 10 * r * s + 5 * r * t * t - 5 * r + 2 * s * t * t) / 20;
				B[15 + 20] = -(6 * r * r * s * t * t - 8 * r * r * s + 5 * r * r * t * t - 5 * r * r - 10 * r * s * t * t + 10 * r * s - 5 * r * t * t + 5 * r + 2 * s * t * t) / 20;
				B[16 + 20] = -(6 * r * r * s * t * t + 10 * r * r * s * t + 2 * r * r * s - 5 * r * r * t * t - 5 * r * r * t - 8 * s * t * t - 10 * s * t + 5 * t * t + 5 * t) / 20;
				B[19 + 20] = -(6 * r * r * s * t * t + 10 * r * r * s * t + 2 * r * r * s + 5 * r * r * t * t + 5 * r * r * t - 8 * s * t * t - 10 * s * t - 5 * t * t - 5 * t) / 20;
				B[ 0 + 20] = (18 * r * r * s * t * t - 10 * r * r * s * t - 4 * r * r * s - 5 * r * r * t * t + 5 * r * r * t - 10 * r * s * t * t + 10 * r * s * t + 5 * r * t * t - 5 * r * t - 4 * s * t * t) / 40;
				B[ 1 + 20] = (18 * r * r * s * t * t - 10 * r * r * s * t - 4 * r * r * s - 5 * r * r * t * t + 5 * r * r * t + 10 * r * s * t * t - 10 * r * s * t - 5 * r * t * t + 5 * r * t - 4 * s * t * t) / 40;
				B[ 2 + 20] = (18 * r * r * s * t * t - 10 * r * r * s * t - 4 * r * r * s + 5 * r * r * t * t - 5 * r * r * t + 10 * r * s * t * t - 10 * r * s * t + 5 * r * t * t - 5 * r * t - 4 * s * t * t) / 40;
				B[ 3 + 20] = (18 * r * r * s * t * t - 10 * r * r * s * t - 4 * r * r * s + 5 * r * r * t * t - 5 * r * r * t - 10 * r * s * t * t + 10 * r * s * t - 5 * r * t * t + 5 * r * t - 4 * s * t * t) / 40;
				B[ 4 + 20] = (18 * r * r * s * t * t + 10 * r * r * s * t - 4 * r * r * s - 5 * r * r * t * t - 5 * r * r * t - 10 * r * s * t * t - 10 * r * s * t + 5 * r * t * t + 5 * r * t - 4 * s * t * t) / 40;
				B[ 5 + 20] = (18 * r * r * s * t * t + 10 * r * r * s * t - 4 * r * r * s - 5 * r * r * t * t - 5 * r * r * t + 10 * r * s * t * t + 10 * r * s * t - 5 * r * t * t - 5 * r * t - 4 * s * t * t) / 40;
				B[ 6 + 20] = (18 * r * r * s * t * t + 10 * r * r * s * t - 4 * r * r * s + 5 * r * r * t * t + 5 * r * r * t + 10 * r * s * t * t + 10 * r * s * t + 5 * r * t * t + 5 * r * t - 4 * s * t * t) / 40;
				B[ 7 + 20] = (18 * r * r * s * t * t + 10 * r * r * s * t - 4 * r * r * s + 5 * r * r * t * t + 5 * r * r * t - 10 * r * s * t * t - 10 * r * s * t - 5 * r * t * t - 5 * r * t - 4 * s * t * t) / 40;
				//gradient t
				B[10 + 40] = -t * (3 * r * r * s * s - 5 * r * r * s + r * r - 5 * r * s * s + 5 * r * s + s * s) / 10;
				B[12 + 40] = -t * (3 * r * r * s * s - 5 * r * r * s + r * r + 5 * r * s * s - 5 * r * s + s * s) / 10;
				B[14 + 40] = -t * (3 * r * r * s * s + 5 * r * r * s + r * r + 5 * r * s * s + 5 * r * s + s * s) / 10;
				B[15 + 40] = -t * (3 * r * r * s * s + 5 * r * r * s + r * r - 5 * r * s * s - 5 * r * s + s * s) / 10;
				B[ 8 + 40] = -(6 * r * r * s * s * t - 5 * r * r * s * s - 10 * r * r * s * t + 5 * r * r * s + 2 * r * r * t - 8 * s * s * t + 5 * s * s + 10 * s * t - 5 * s) / 20;
				B[ 9 + 40] = -(6 * r * r * s * s * t - 5 * r * r * s * s - 8 * r * r * t + 5 * r * r - 10 * r * s * s * t + 5 * r * s * s + 10 * r * t - 5 * r + 2 * s * s * t) / 20;
				B[11 + 40] = -(6 * r * r * s * s * t - 5 * r * r * s * s - 8 * r * r * t + 5 * r * r + 10 * r * s * s * t - 5 * r * s * s - 10 * r * t + 5 * r + 2 * s * s * t) / 20;
				B[13 + 40] = -(6 * r * r * s * s * t - 5 * r * r * s * s + 10 * r * r * s * t - 5 * r * r * s + 2 * r * r * t - 8 * s * s * t + 5 * s * s - 10 * s * t + 5 * s) / 20;
				B[16 + 40] = -(6 * r * r * s * s * t + 5 * r * r * s * s - 10 * r * r * s * t - 5 * r * r * s + 2 * r * r * t - 8 * s * s * t - 5 * s * s + 10 * s * t + 5 * s) / 20;
				B[17 + 40] = -(6 * r * r * s * s * t + 5 * r * r * s * s - 8 * r * r * t - 5 * r * r - 10 * r * s * s * t - 5 * r * s * s + 10 * r * t + 5 * r + 2 * s * s * t) / 20;
				B[18 + 40] = -(6 * r * r * s * s * t + 5 * r * r * s * s - 8 * r * r * t - 5 * r * r + 10 * r * s * s * t + 5 * r * s * s - 10 * r * t - 5 * r + 2 * s * s * t) / 20;
				B[19 + 40] = -(6 * r * r * s * s * t + 5 * r * r * s * s + 10 * r * r * s * t + 5 * r * r * s + 2 * r * r * t - 8 * s * s * t - 5 * s * s - 10 * s * t - 5 * s) / 20;
				B[ 0 + 40] = (18 * r * r * s * s * t - 5 * r * r * s * s - 10 * r * r * s * t + 5 * r * r * s - 4 * r * r * t - 10 * r * s * s * t + 5 * r * s * s + 10 * r * s * t - 5 * r * s - 4 * s * s * t) / 40;
				B[ 1 + 40] = (18 * r * r * s * s * t - 5 * r * r * s * s - 10 * r * r * s * t + 5 * r * r * s - 4 * r * r * t + 10 * r * s * s * t - 5 * r * s * s - 10 * r * s * t + 5 * r * s - 4 * s * s * t) / 40;
				B[ 2 + 40] = (18 * r * r * s * s * t - 5 * r * r * s * s + 10 * r * r * s * t - 5 * r * r * s - 4 * r * r * t + 10 * r * s * s * t - 5 * r * s * s + 10 * r * s * t - 5 * r * s - 4 * s * s * t) / 40;
				B[ 3 + 40] = (18 * r * r * s * s * t - 5 * r * r * s * s + 10 * r * r * s * t - 5 * r * r * s - 4 * r * r * t - 10 * r * s * s * t + 5 * r * s * s - 10 * r * s * t + 5 * r * s - 4 * s * s * t) / 40;
				B[ 4 + 40] = (18 * r * r * s * s * t + 5 * r * r * s * s - 10 * r * r * s * t - 5 * r * r * s - 4 * r * r * t - 10 * r * s * s * t - 5 * r * s * s + 10 * r * s * t + 5 * r * s - 4 * s * s * t) / 40;
				B[ 5 + 40] = (18 * r * r * s * s * t + 5 * r * r * s * s - 10 * r * r * s * t - 5 * r * r * s - 4 * r * r * t + 10 * r * s * s * t + 5 * r * s * s - 10 * r * s * t - 5 * r * s - 4 * s * s * t) / 40;
				B[ 6 + 40] = (18 * r * r * s * s * t + 5 * r * r * s * s + 10 * r * r * s * t + 5 * r * r * s - 4 * r * r * t + 10 * r * s * s * t + 5 * r * s * s + 10 * r * s * t + 5 * r * s - 4 * s * s * t) / 40;
				B[ 7 + 40] = (18 * r * r * s * s * t + 5 * r * r * s * s + 10 * r * r * s * t + 5 * r * r * s - 4 * r * r * t - 10 * r * s * s * t - 5 * r * s * s - 10 * r * s * t - 5 * r * s - 4 * s * s * t) / 40;
			}
		}
	}
}
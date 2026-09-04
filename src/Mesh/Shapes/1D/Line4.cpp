//FEA
#include "FEA/inc/Mesh/Shapes/1D/Line4.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Line4::Line4(void) : Line(2)
			{
				return;
			}

			//destructor
			Line4::~Line4(void)
			{
				return;
			}

			//geometry
			uint32_t Line4::vertices(void) const
			{
				return 4U;
			}

			//shape
			void Line4::function(double* N, const double* p) const
			{
				//data
				const double r = p[0];
				//shape
				N[0] = (-1 + r + 9 * r * r - 9 * r * r * r) / 16;
				N[1] = (-1 - r + 9 * r * r + 9 * r * r * r) / 16;
				N[2] = (+9 - 27 * r - 9 * r * r + 27 * r * r * r) / 16; 
				N[3] = (+9 + 27 * r - 9 * r * r - 27 * r * r * r) / 16;
			}
			void Line4::gradient(double* B, const double* p) const
			{
				//data
				const double r = p[0];
				//gradient
				B[0] = (+1 + 18 * r - 27 * r * r) / 16;
				B[1] = (-1 + 18 * r + 27 * r * r) / 16;
				B[2] = (-27 - 18 * r + 81 * r * r) / 16;
				B[3] = (+27 - 18 * r - 81 * r * r) / 16;
			}
		}
	}
}
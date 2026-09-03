//FEA
#include "FEA/inc/Mesh/Shapes/3D/Tetra4.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tetra4::Tetra4(void) : Tetra(1)
			{
				return;
			}

			//destructor
			Tetra4::~Tetra4(void)
			{
				return;
			}

			//integration
			void Tetra4::function(double* N, const double* s) const
			{
				return;
			}
			void Tetra4::gradient(double* B, const double* s) const
			{
				return;
			}
		}
	}
}
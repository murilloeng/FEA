//FEA
#include "FEA/inc/Mesh/Shapes/3D/Tetra10.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tetra10::Tetra10(void) : Tetra(1)
			{
				return;
			}

			//destructor
			Tetra10::~Tetra10(void)
			{
				return;
			}

			//integration
			void Tetra10::function(double* N, const double* s) const
			{
				return;
			}
			void Tetra10::gradient(double* B, const double* s) const
			{
				return;
			}
		}
	}
}
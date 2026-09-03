//FEA
#include "FEA/inc/Mesh/Shapes/3D/Tetra.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tetra::Tetra(uint32_t order) : Volume(order)
			{
				return;
			}

			//destructor
			Tetra::~Tetra(void)
			{
				return;
			}

			//integration
			void Tetra::point(double& w, double* s, uint32_t index) const
			{
				return;
			}
		}
	}
}
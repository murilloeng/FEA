//FEA
#include "FEA/inc/Mesh/Shapes/3D/Tetrahedron.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Tetrahedron::Tetrahedron(uint32_t order) : Volume(order)
			{
				return;
			}

			//destructor
			Tetrahedron::~Tetrahedron(void)
			{
				return;
			}

			//geometry
			uint32_t Tetrahedron::edges(void) const
			{
				return 6U;
			}
			uint32_t Tetrahedron::faces(void) const
			{
				return 4U;
			}

			//integration
			void Tetrahedron::point(double& w, double* s, uint32_t index) const
			{
				return;
			}
		}
	}
}
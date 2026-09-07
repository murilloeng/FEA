//FEA
#include "FEA/inc/Mesh/Shapes/3D/Hexahedron.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Hexahedron::Hexahedron(uint32_t order) : Volume(order)
			{
				return;
			}

			//destructor
			Hexahedron::~Hexahedron(void)
			{
				return;
			}

			//geometry
			uint32_t Hexahedron::edges(void) const
			{
				return 12U;
			}
			uint32_t Hexahedron::faces(void) const
			{
				return 6U;
			}

			//integration
			void Hexahedron::point(double& w, double* s, uint32_t index) const
			{
				return;
			}
		}
	}
}
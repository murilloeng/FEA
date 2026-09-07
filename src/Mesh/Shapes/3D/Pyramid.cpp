//FEA
#include "FEA/inc/Mesh/Shapes/3D/Pyramid.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Pyramid::Pyramid(uint32_t order) : Volume(order)
			{
				return;
			}

			//destructor
			Pyramid::~Pyramid(void)
			{
				return;
			}

			//geometry
			uint32_t Pyramid::edges(void) const
			{
				return 8U;
			}
			uint32_t Pyramid::faces(void) const
			{
				return 5U;
			}

			//integration
			void Pyramid::point(double& w, double* s, uint32_t index) const
			{
				return;
			}
		}
	}
}
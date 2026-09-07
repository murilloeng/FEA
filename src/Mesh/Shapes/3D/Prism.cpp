//FEA
#include "FEA/inc/Mesh/Shapes/3D/Prism.hpp"

namespace fea
{
	namespace mesh
	{
		namespace shapes
		{
			//constructor
			Prism::Prism(uint32_t order) : Volume(order)
			{
				return;
			}

			//destructor
			Prism::~Prism(void)
			{
				return;
			}

			//geometry
			uint32_t Prism::edges(void) const
			{
				return 9U;
			}
			uint32_t Prism::faces(void) const
			{
				return 5U;
			}

			//integration
			void Prism::point(double& w, double* s, uint32_t index) const
			{
				return;
			}
		}
	}
}
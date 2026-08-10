//FEA
#include "FEA/inc/Mesh/Joints/Rigid3D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			//constructors
			Rigid3D::Rigid3D(void)
			{
				return;
			}

			//destructor
			Rigid3D::~Rigid3D(void)
			{
				return;
			}

			//analysis
			void Rigid3D::create_constraints(void) const
			{
				return;
			}
			void Rigid3D::create_dependencies(void) const
			{
				return;
			}

			//draw
			void Rigid3D::draw_setup(draw::Data&) const
			{
				return;
			}
			void Rigid3D::draw_update(draw::Data&) const
			{
				return;
			}
		}
	}
}
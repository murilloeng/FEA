//FEA
#include "FEA/inc/Mesh/Joints/Rigid2D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			//constructors
			Rigid2D::Rigid2D(void)
			{
				return;
			}

			//destructor
			Rigid2D::~Rigid2D(void)
			{
				return;
			}

			//analysis
			void Rigid2D::create_constraints(void) const
			{
				return;
			}
			void Rigid2D::create_dependencies(void) const
			{
				return;
			}

			//draw
			void Rigid2D::draw_setup(draw::Data&) const
			{
				return;
			}
			void Rigid2D::draw_update(draw::Data&) const
			{
				return;
			}
		}
	}
}
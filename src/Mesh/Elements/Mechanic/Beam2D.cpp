//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam2D::Beam2D(void)
			{
				return;
			}

			//destructor
			Beam2D::~Beam2D(void)
			{
				return;
			}

			//data
			uint32_t Beam2D::dof_set(uint32_t) const
			{
				return
					1 << uint32_t(nodes::DOF::Rotation_3)|
					1 << uint32_t(nodes::DOF::Translation_1)|
					1 << uint32_t(nodes::DOF::Translation_2);
			}

			//tangents
			void Beam2D::inertia(double*) const
			{
				return;
			}
			void Beam2D::damping(double*) const
			{
				return;
			}
			void Beam2D::stiffness(double*) const
			{
				return;
			}

			//forces
			void Beam2D::internal_force(double*) const
			{
				return;
			}

			//analysis
			void Beam2D::compute(void)
			{
				return;
			}
		}
	}
}
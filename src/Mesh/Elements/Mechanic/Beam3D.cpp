//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam3D::Beam3D(void)
			{
				return;
			}

			//destructor
			Beam3D::~Beam3D(void)
			{
				return;
			}

			//data
			uint32_t Beam3D::dof_set(uint32_t) const
			{
				return
					1 << uint32_t(nodes::DOF::Rotation_1)|
					1 << uint32_t(nodes::DOF::Rotation_2)|
					1 << uint32_t(nodes::DOF::Rotation_3)|
					1 << uint32_t(nodes::DOF::Translation_1)|
					1 << uint32_t(nodes::DOF::Translation_2)|
					1 << uint32_t(nodes::DOF::Translation_3);
			}

			//tangents
			void Beam3D::inertia(double*) const
			{
				return;
			}
			void Beam3D::damping(double*) const
			{
				return;
			}
			void Beam3D::stiffness(double*) const
			{
				return;
			}

			//forces
			void Beam3D::internal_force(double*) const
			{
				return;
			}

			//analysis
			void Beam3D::compute(void)
			{
				return;
			}
		}
	}
}
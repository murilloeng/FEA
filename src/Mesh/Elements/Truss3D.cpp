//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Elements/Truss3D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Truss3D::Truss3D(void)
			{
				return;
			}
	
			//destructor
			Truss3D::~Truss3D(void)
			{
				return;
			}

			//type
			Type Truss3D::type(void) const
			{
				return Type::Truss3D;
			}

			//dof
			uint32_t Truss3D::dof(uint32_t) const
			{
				return
					uint32_t(DOF::Translation_1)|
					uint32_t(DOF::Translation_2)|
					uint32_t(DOF::Translation_3);
			}

			//tangents
			void Truss3D::inertia(double*) const
			{
				return;
			}
			void Truss3D::damping(double*) const
			{
				return;
			}
			void Truss3D::stiffness(double*) const
			{
				return;
			}

			//forces
			void Truss3D::internal_force(double*) const
			{
				return;
			}
		}
	}
}
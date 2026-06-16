//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Elements/Truss2D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Truss2D::Truss2D(void)
			{
				return;
			}
	
			//destructor
			Truss2D::~Truss2D(void)
			{
				return;
			}

			//type
			Type Truss2D::type(void) const
			{
				return Type::Truss2D;
			}

			//dof
			uint32_t Truss2D::dof(uint32_t) const
			{
				return 
					uint32_t(DOF::Translation_1)|
					uint32_t(DOF::Translation_2);
			}

			//tangents
			void Truss2D::inertia(double*) const
			{
				return;
			}
			void Truss2D::damping(double*) const
			{
				return;
			}
			void Truss2D::stiffness(double*) const
			{
				return;
			}

			//forces
			void Truss2D::internal_force(double*) const
			{
				return;
			}
		}
	}
}
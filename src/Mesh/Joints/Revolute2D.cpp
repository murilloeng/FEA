//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Joints/Revolute2D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			//constructors
			Revolute2D::Revolute2D(void)
			{
				return;
			}

			//destructor
			Revolute2D::~Revolute2D(void)
			{
				return;
			}

			//analysis
			void Revolute2D::create_constraints(void) const
			{
				return;
			}
			void Revolute2D::create_dependencies(void) const
			{
				m_mesh->model()->boundary()->create_dependency(m_nodes[0], nodes::DOF::Translation_1, m_nodes[1], nodes::DOF::Translation_1);
				m_mesh->model()->boundary()->create_dependency(m_nodes[0], nodes::DOF::Translation_2, m_nodes[1], nodes::DOF::Translation_2);
			}

			//draw
			void Revolute2D::draw_setup(draw::Data&) const
			{
				return;
			}
			void Revolute2D::draw_update(draw::Data&) const
			{
				return;
			}
		}
	}
}
//std
#include <stdexcept>

//FEA
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Nodal/Spring.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Spring::Spring(void) : m_dof{nodes::DOF::Last}, m_inertia{0}, m_damping{0}, m_stiffness{0}
			{
				return;
			}
			
			//destructor
			Spring::~Spring(void)
			{
				return;
			}
			
			//data
			nodes::DOF Spring::dof(void) const
			{
				return m_dof;
			}
			nodes::DOF Spring::dof(nodes::DOF dof)
			{
				return m_dof = dof;
			}

			double Spring::inertia(void) const
			{
				return m_inertia;
			}
			double Spring::inertia(double inertia)
			{
				return m_inertia = inertia;
			}

			double Spring::damping(void) const
			{
				return m_damping;
			}
			double Spring::damping(double damping)
			{
				return m_damping = damping;
			}

			double Spring::stiffness(void) const
			{
				return m_stiffness;
			}
			double Spring::stiffness(double stiffness)
			{
				return m_stiffness = stiffness;
			}

			uint32_t Spring::dof_set(uint32_t) const
			{
				return 1 << uint32_t(m_dof);
			}

			//tangents
			void Spring::inertia(double* M) const
			{
				M[0] = m_inertia;
			}
			void Spring::damping(double* C) const
			{
				C[0] = m_damping;
			}
			void Spring::stiffness(double* K) const
			{
				K[0] = m_stiffness;
			}

			//forces
			void Spring::internal_force(double* f) const
			{
				//data
				const double x = node(0)->state(m_dof);
				const double v = node(0)->velocity(m_dof);
				//force
				f[0] = m_damping * v + m_stiffness * x;
			}

			//analysis
			void Spring::check(void)
			{
				Element::check();
				if(m_nodes.size() != 1)
				{
					throw std::runtime_error("Error: Spring element must have excatly one node!");
				}
				if(m_dof == nodes::DOF::Last)
				{
					throw std::runtime_error("Error: Spring element dof is unset!");
				}
			}
			void Spring::compute(void)
			{
				return;
			}
		}
	}
}
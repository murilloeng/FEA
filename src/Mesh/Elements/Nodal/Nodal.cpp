//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Nodal/Nodal.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Nodal::Nodal(void) : m_dof{nodes::DOF::Last}, m_inertia{0}, m_damping{0}, m_stiffness{0}
			{
				return;
			}

			//destructor
			Nodal::~Nodal(void)
			{
				return;
			}

			//data
			nodes::DOF Nodal::dof(void) const
			{
				return m_dof;
			}
			nodes::DOF Nodal::dof(nodes::DOF dof)
			{
				return m_dof = dof;
			}

			double Nodal::inertia(void) const
			{
				return m_inertia;
			}
			double Nodal::inertia(double inertia)
			{
				return m_inertia = inertia;
			}

			double Nodal::damping(void) const
			{
				return m_damping;
			}
			double Nodal::damping(double damping)
			{
				return m_damping = damping;
			}

			double Nodal::stiffness(void) const
			{
				return m_stiffness;
			}
			double Nodal::stiffness(double stiffness)
			{
				return m_stiffness = stiffness;
			}

			uint32_t Nodal::dof_set(uint32_t) const
			{
				return 1 << uint32_t(m_dof);
			}

			//tangents
			void Nodal::inertia(double* M) const
			{
				M[0] = m_inertia;
			}
			void Nodal::damping(double* C) const
			{
				C[0] = m_damping;
			}
			void Nodal::stiffness(double* K) const
			{
				K[0] = m_stiffness;
			}

			//forces
			void Nodal::internal_force(double* f) const
			{
				//data
				const double x = m_mesh->model()->analysis()->solver()->state_new() ? node(0)->state(m_dof) : 0;
				const double v = m_mesh->model()->analysis()->solver()->velocity_new() ? node(0)->velocity(m_dof) : 0;
				//force
				f[0] = m_damping * v + m_stiffness * x;
			}

			//analysis
			void Nodal::check(void)
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
			void Nodal::compute(void)
			{
				return;
			}
		}
	}
}
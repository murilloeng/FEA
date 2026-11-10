//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Nodal/Nodal.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Request.hpp"
#include "FEA/inc/Analysis/Assembler/Compute.hpp"

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

			//analysis
			void Nodal::check(void)
			{
				Element::check();
				if(m_nodes.size() != 1)
				{
					throw std::runtime_error("Error: Nodal element must have excatly one node!");
				}
				if(m_dof == nodes::DOF::Last)
				{
					throw std::runtime_error("Error: Nodal element dof is unset!");
				}
			}
			void Nodal::compute(analysis::Compute& compute)
			{
				//data
				const double M = m_inertia;
				const double C = m_damping;
				const double K = m_stiffness;
				const double x = node(0)->state(m_dof);
				const double v = node(0)->velocity(m_dof);
				//tangents
				if(compute.m_request->m_M) compute.m_M[0] = M;
				if(compute.m_request->m_C) compute.m_C[0] = C;
				if(compute.m_request->m_K) compute.m_K[0] = K;
				//internal energy
				if(compute.m_request->m_U) compute.m_U = K * x * x / 2;
				//internal force
				if(compute.m_request->m_f) compute.m_f[0] = K * x + C * v;
			}
		}
	}
}
//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Support::Support(uint32_t node, mesh::nodes::DOF dof) : m_node{node}, m_dof_index{0}, m_dof{dof}
		{
			return;
		}
	
		//destructor
		Support::~Support(void)
		{
			return;
		}

		//data
		double Support::state(void)
		{
			return m_state ? m_state(m_boundary->model()->analysis()->solver()->m_t_new) : 0;
		}
		double Support::velocity(void)
		{
			return m_velocity ? m_velocity(m_boundary->model()->analysis()->solver()->m_t_new) : 0;
		}
		double Support::acceleration(void)
		{
			return m_acceleration ? m_acceleration(m_boundary->model()->analysis()->solver()->m_t_new) : 0;
		}

		uint32_t Support::index_node(void) const
		{
			return m_node;
		}

		mesh::nodes::Node* Support::node(void) const
		{
			return m_boundary->model()->mesh()->node(m_node);
		}
		mesh::nodes::Node* Support::node(uint32_t node)
		{
			return m_boundary->model()->mesh()->node(m_node = node);
		}

		mesh::nodes::DOF Support::dof(void) const
		{
			return m_dof;
		}
		mesh::nodes::DOF Support::dof(mesh::nodes::DOF dof)
		{
			return m_dof = dof;
		}

		//analysis
		void Support::check(void)
		{
			if(m_node >= m_boundary->model()->mesh()->nodes().size())
			{
				throw std::runtime_error("Error: Suuport's node is out of range!");
			}
		}
		void Support::setup(void)
		{
			m_dof_index = m_boundary->model()->mesh()->node(m_node)->dof_index(m_dof);
		}

		//static
		Boundary* Support::m_boundary = nullptr;
	}
}
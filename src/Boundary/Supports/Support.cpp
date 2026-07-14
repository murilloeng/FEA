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

		//serialization
		void Support::save(FILE* file) const
		{
			fprintf(file, "Index: %d Node: %d DOF: %d ", m_index, m_node, uint32_t(m_dof));
		}

		//data
		Support::Function Support::state(void) const
		{
			return m_state;
		}
		Support::Function Support::state(Function state)
		{
			return m_state = state;
		}

		Support::Function Support::velocity(void) const
		{
			return m_velocity;
		}
		Support::Function Support::velocity(Function velocity)
		{
			return m_velocity = velocity;
		}

		Support::Function Support::acceleration(void) const
		{
			return m_acceleration;
		}
		Support::Function Support::acceleration(Function acceleration)
		{
			return m_acceleration = acceleration;
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

		//time
		double Support::state(double t) const
		{
			return m_state ? m_state(t) : 0;
		}
		double Support::velocity(double t) const
		{
			return m_velocity ? m_velocity(t) : 0;
		}
		double Support::acceleration(double t) const
		{
			return m_acceleration ? m_acceleration(t) : 0;
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
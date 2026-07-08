//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Initials/Initial.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Initial::Initial(void) : m_state{0}, m_velocity{0}, m_node{0}, m_dof_index{0}, m_dof{mesh::nodes::DOF::Translation_1}
		{
			return;
		}
	
		//destructor
		Initial::~Initial(void)
		{
			return;
		}

		//data
		double Initial::state(void) const
		{
			return m_state;
		}
		double Initial::state(double state)
		{
			return m_state = state;
		}

		double Initial::velocity(void) const
		{
			return m_velocity;
		}
		double Initial::velocity(double velocity)
		{
			return m_velocity = velocity;
		}

		uint32_t Initial::index_node(void) const
		{
			return m_node;
		}

		mesh::nodes::DOF Initial::dof(void) const
		{
			return m_dof;
		}
		mesh::nodes::DOF Initial::dof(mesh::nodes::DOF dof)
		{
			return m_dof = dof;
		}

		mesh::nodes::Node* Initial::node(void) const
		{
			return m_boundary->model()->mesh()->node(m_node);
		}
		mesh::nodes::Node* Initial::node(uint32_t node)
		{
			return m_boundary->model()->mesh()->node(m_node = node);
		}

		//analysis
		void Initial::check(void)
		{
			if(m_node >= m_boundary->model()->mesh()->nodes().size())
			{
				throw std::runtime_error("Error: Initial's node is out of range!");
			}
		}
		void Initial::setup(void)
		{
			m_dof_index = m_boundary->model()->mesh()->node(m_node)->dof_index(m_dof);
		}

		//static
		Boundary* Initial::m_boundary = nullptr;
	}
}
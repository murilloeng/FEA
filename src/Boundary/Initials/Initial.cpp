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

		//analysis
		void Initial::check(void)
		{
			if(m_node >= m_boundary->model()->m_mesh->nodes().size())
			{
				throw std::runtime_error("Error: Initial's node is out of range!");
			}
		}
		void Initial::setup(void)
		{
			m_dof_index = m_boundary->model()->m_mesh->node(m_node)->dof_index(m_dof);
		}

		//static
		Boundary* Initial::m_boundary = nullptr;
	}
}
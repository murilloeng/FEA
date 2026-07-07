//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

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

		//analysis
		void Support::check(void)
		{
			if(m_node >= m_boundary->m_model->m_mesh->m_nodes.size())
			{
				throw std::runtime_error("Error: Suuport's node is out of range!");
			}
		}
		void Support::setup(void)
		{
			m_dof_index = m_boundary->m_model->m_mesh->m_nodes[m_node]->dof_index(m_dof);
		}

		//data
		mesh::nodes::Node* Support::node(void) const
		{
			return m_boundary->m_model->m_mesh->m_nodes[m_node];
		}

		//static
		Boundary* Support::m_boundary = nullptr;
	}
}
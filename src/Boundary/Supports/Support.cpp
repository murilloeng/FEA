//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Support::Support(void) : m_node{0}, m_dof_index{0}, m_dof{mesh::nodes::DOF::Translation_1}
		{
			return;
		}
		
		//destructor
		Support::~Support(void)
		{
			return;
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
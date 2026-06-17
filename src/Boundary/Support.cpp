//FEA
#include "FEA/inc/Model.hpp"
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Boundary/Support.hpp"
#include "FEA/inc/Boundary/Boundary.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Support::Support(void) : m_node{0}, m_index{0}, m_dof{mesh::nodes::DOF::Translation_1}
		{
			return;
		}
		
		//destructor
		Support::~Support(void)
		{
			return;
		}

		//data
		uint32_t Support::index(void) const
		{
			return m_index;
		}

		Boundary* Support::boundary(void)
		{
			return m_boundary;
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
	}
}
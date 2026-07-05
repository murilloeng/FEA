//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
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

		//static
		Boundary* Support::m_boundary = nullptr;
	}
}
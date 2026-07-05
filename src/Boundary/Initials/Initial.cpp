//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Boundary/Initials/Initial.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Initial::Initial(void) : m_state{0}, m_velocity{0}, m_node{0}, m_dof{mesh::nodes::DOF::Translation_1}
		{
			return;
		}
		
		//destructor
		Initial::~Initial(void)
		{
			return;
		}

		//static
		Boundary* Initial::m_boundary = nullptr;
	}
}
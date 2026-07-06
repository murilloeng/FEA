//FEA
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Constraint::Constraint(void) : m_dof_index{0}
		{
			return;
		}
		
		//destructor
		Constraint::~Constraint(void)
		{
			return;
		}

		//analysis
		void Constraint::setup_dof(uint32_t& dof_counter)
		{
			m_dof_index = dof_counter++;
		}

		//static
		Boundary* Constraint::m_boundary = nullptr;
	}
}
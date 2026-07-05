//FEA
#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Boundary::Boundary(void)
		{
			Support::m_boundary = this;
			LoadCase::m_boundary = this;
			LoadCombination::m_boundary = this;
		}
		
		//destructor
		Boundary::~Boundary(void)
		{
			for(const Support* support : m_supports) delete support;
			for(const LoadCase* load_case : m_load_cases) delete load_case;
			for(const LoadCombination* load_combination : m_load_combinations) delete load_combination;
		}

		//static
		Model* Boundary::m_model = nullptr;
	}
}
//FEA
#include "FEA/inc/Analysis/Assembler/Request.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Request::Request(void) : 
			m_T{false}, m_U{false}, m_V{false},
			m_f{false}, m_K{false}, m_C{false}, m_M{false}
		{
			return;
		}

		//setup
		void Request::setup(void)
		{
			m_T = m_U = m_V = false;
			m_f = m_K = m_C = m_M = false;
		}
	}
}
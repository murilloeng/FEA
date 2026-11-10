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
	}
}
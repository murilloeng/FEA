//FEA
#include "FEA/inc/Analysis/Analysis.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Analysis::Analysis(void)
		{
			return;
		}
		
		//destructor
		Analysis::~Analysis(void)
		{
			return;
		}

		//static
		Model* Analysis::m_model = nullptr;
	}
}
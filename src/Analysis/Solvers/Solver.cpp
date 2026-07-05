//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Solver::Solver(void)
		{
			return;
		}
		
		//destructor
		Solver::~Solver(void)
		{
			return;
		}

		//static
		Analysis* Solver::m_analysis = nullptr;
	}
}
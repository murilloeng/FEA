//FEA
#include "FEA/inc/Analysis/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Solver::Solver(void) : m_R{nullptr}
		{
			return;
		}

		//destructor
		Solver::~Solver(void)
		{
			delete[] m_R;
		}

		//data
		const double* Solver::reactions(void) const
		{
			return m_R;
		}

		//static data
		Analysis* Solver::m_analysis = nullptr;
	}
}
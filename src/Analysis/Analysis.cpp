//FEA
#include "FEA/inc/Analysis/Solver.hpp"
#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Analysis::Analysis(void) : m_solver{nullptr}, m_assembler{new Assembler}
		{
			return;
		}
		
		//destructor
		Analysis::~Analysis(void)
		{
			delete m_solver;
			delete m_assembler;
		}

		//static
		Model* Analysis::m_model = nullptr;
	}
}
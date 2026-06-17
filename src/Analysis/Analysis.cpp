//FEA
#include "FEA/inc/Analysis/Solver.hpp"
#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Analysis::Analysis(void) : m_solver{nullptr}, m_assembler{nullptr}
		{
			return;
		}

		//destructor
		Analysis::~Analysis(void)
		{
			delete m_solver;
			delete m_assembler;
		}

		//data
		Model* Analysis::model(void)
		{
			return m_model;
		}
		Solver* Analysis::solver(void) const
		{
			return m_solver;
		}
		Assembler* Analysis::assembler(void) const
		{
			return m_assembler;
		}

		//analysis
		void Analysis::check(void)
		{
			return;
		}
		void Analysis::setup(void)
		{
			return;
		}
		void Analysis::record(void)
		{
			return;
		}
		void Analysis::update(void)
		{
			return;
		}
		void Analysis::restore(void)
		{
			return;
		}
		void Analysis::compute(void)
		{
			return;
		}

		//static data
		Model* Analysis::m_model = nullptr;
	}
}
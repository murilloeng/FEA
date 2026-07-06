#pragma once

//Math
#include "Math/inc/Solvers/Solver.hpp"

//FEA
#include "FEA/inc/Analysis/Solvers/WatchDOF.hpp"

namespace fea
{
	namespace analysis
	{
		class Analysis;
	}
}

namespace fea
{
	namespace analysis
	{
		class Solver : virtual public math::solvers::Solver
		{
		public:
			//constructor
			Solver(void);

			//destructor
			~Solver(void);

			//analysis
			void check(void) override;
			void setup(void) override;

			//data
			WatchDOF m_watch_dof;
			static Analysis* m_analysis;
		};
	}
}
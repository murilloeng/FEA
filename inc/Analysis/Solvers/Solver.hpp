#pragma once

//Math
#include "Math/inc/Solvers/Solver.hpp"

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

			//data
			static Analysis* m_analysis;
		};
	}
}
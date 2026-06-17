#pragma once

//std
#include <cstdint>

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
			const double* reactions(void) const;

		private:
			//friends
			friend class Analysis;

			//data
			double* m_R;
			static Analysis* m_analysis;
		};
	}
}
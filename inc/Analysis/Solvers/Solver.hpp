#pragma once

//std
#include <cstdint>

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
		protected:
			//constructor
			Solver(void);

			//destructor
			~Solver(void);

		public:
			//data
			WatchDOF& watch_dof(void);

			uint32_t load_combination(uint32_t);
			uint32_t load_combination(void) const;

		protected:
			//analysis
			void check(void) override;
			void setup(void) override;
			void allocate(void) override;

			//data
			WatchDOF m_watch_dof;
			uint32_t m_load_combination;
			static Analysis* m_analysis;

			//friends
			friend class fea::analysis::Analysis;
		};
	}
}
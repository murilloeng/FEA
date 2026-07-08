#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		class StaticLinear : public Solver
		{
		private:
			//constructor
			StaticLinear(void);

			//destructor
			~StaticLinear(void);

		public:
			//data
			uint32_t state_set(void) const override;
			uint32_t force_set(void) const override;
			uint32_t tangent_set(void) const override;

			//solve
			void solve(void) override;

			//friends
			friend class fea::analysis::Analysis;
		};
	}
}
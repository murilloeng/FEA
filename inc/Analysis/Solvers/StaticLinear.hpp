#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		class StaticLinear : public Solver
		{
		public:
			//constructor
			StaticLinear(void);

			//destructor
			~StaticLinear(void);

			//data
			uint32_t state_set(void) const override;
			uint32_t force_set(void) const override;
			uint32_t tangent_set(void) const override;

			//solve
			void solve(void) override;
		};
	}
}
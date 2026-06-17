#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class StaticLinear : public Solver
			{
			private:
				//constructors
				StaticLinear(void);

				//destructor
				~StaticLinear(void) override;

			public:
				//type
				Type type(void) const override;

				//sets
				uint32_t state_set(void) const override;
				uint32_t force_set(void) const override;
				uint32_t tangent_set(void) const override;

			private:
				//analysis
				bool solve(void) override;

				//solve
				void solve_setup(void);
				bool solve_state(void);
				bool solve_apply(void);

				//compute
				void compute_state(void);
				void compute_supports(void);

				//data
				double* m_xk;

				//friends
				friend class Solver;
			};
		}
	}
}
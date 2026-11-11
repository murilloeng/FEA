#pragma once

//Math
#include "Math/inc/Solvers/NewtonRaphson.hpp"

//FEA
#include "FEA/inc/Analysis/Solvers/Incremental.hpp"

namespace fea
{
	namespace analysis
	{
		class StaticNonlinear : virtual public Incremental, virtual public math::solvers::NewtonRaphson
		{
		private:
			//constructor
			StaticNonlinear(void) = default;

			//destructor
			~StaticNonlinear(void) = default;

			//data
			uint32_t forces_set(void) const override;

			//analysis
			void check(void) override;
			void setup(void) override;
			void print(void) override;

			//solve
			void solve(void) override;

			//compute
			void compute_stability(void);

			//friends
			friend class fea::analysis::Analysis;
		};
	}
}
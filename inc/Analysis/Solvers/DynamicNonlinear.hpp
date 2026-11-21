#pragma once

//Math
#include "Math/inc/Solvers/Newmark.hpp"

//FEA
#include "FEA/inc/Analysis/Solvers/Incremental.hpp"

namespace fea
{
	namespace analysis
	{
		class DynamicNonlinear : virtual public Incremental, virtual public math::solvers::Newmark
		{
		private:
			//constructor
			DynamicNonlinear(void) = default;

			//destructor
			~DynamicNonlinear(void) = default;

			//analysis
			void check(void) override;
			void setup(void) override;
			void print(void) override;
			void compute(void) override;

			//solve
			void solve(void) override;

			//friends
			friend class fea::analysis::Analysis;
		};
	}
}
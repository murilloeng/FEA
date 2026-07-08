#pragma once

//Math
#include "Math/inc/Solvers/NewtonRaphson.hpp"

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		class StaticNonlinear : virtual public Solver, virtual public math::solvers::NewtonRaphson
		{
		public:
			//constructor
			StaticNonlinear(void);

			//destructor
			~StaticNonlinear(void);

			//analysis
			void check(void) override;
			void setup(void) override;
			void print(void) override;

			//solve
			void solve(void) override;
		};
	}
}
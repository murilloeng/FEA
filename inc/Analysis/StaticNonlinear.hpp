#pragma once

//Math
#include "Math/inc/Solvers/NewtonRaphson.hpp"

//FEA
#include "FEA/inc/Analysis/Solver.hpp"

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
		};
	}
}
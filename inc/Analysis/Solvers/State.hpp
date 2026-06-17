#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class State : public Nonlinear
			{
			private:
				//constructors
				State(void);

				//destructor
				~State(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//type
				Type type(void) const override;

				//sets
				uint32_t state_set(void) const override;
				uint32_t force_set(void) const override;
				uint32_t tangent_set(void) const override;

			private:
				//analysis
				void setup(void) override;
				bool solve(void) override;

				//compute
				void compute_reactions(void);

				//friends
				friend class Solver;
			};
		}
	}
}
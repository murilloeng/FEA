#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Transient.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class TransientLinear : public Transient
			{
			private:
				//constructors
				TransientLinear(void);

				//destructor
				~TransientLinear(void) override;

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
				void record(void) override;

				//friends
				friend class Solver;
			};
		}
	}
}
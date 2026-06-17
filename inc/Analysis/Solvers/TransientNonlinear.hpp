#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Transient.hpp"
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class TransientNonlinear : public virtual Transient, public virtual Nonlinear
			{
			private:
				//constructors
				TransientNonlinear(void);

				//destructor
				~TransientNonlinear(void) override;

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
				bool solve(void) override;
				void record(void) override;

				//friends
				friend class Solver;
			};
		}
	}
}
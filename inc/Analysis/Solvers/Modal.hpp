#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Eigen.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Modal : public Eigen
			{
			private:
				//constructors
				Modal(void);

				//destructor
				~Modal(void) override;

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
				//solve
				void setup(void) override;
				bool solve(void) override;
				void record(void) override;

				//compute
				void compute_state(void);

				//friends
				friend class Solver;
			};
		}
	}
}
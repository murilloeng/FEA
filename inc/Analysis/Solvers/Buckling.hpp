#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Eigen.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Buckling : public Eigen
			{
			private:
				//constructors
				Buckling(void);

				//destructor
				~Buckling(void) override;

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

				void compute_state(void);
				bool compute_buckling(void);
				void compute_reactions(void);

				//friends
				friend class Solver;
			};
		}
	}
}
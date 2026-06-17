#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Dynamic.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class DynamicLinear : public Dynamic
			{
			public:
				//constructors
				DynamicLinear(void);

				//destructor
				~DynamicLinear(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//type
				Type type(void) const override;

			private:
				//analysis
				void setup(void) override;

				bool solve_newmark(void);
				bool solve_runge_kutta(void);

				void compute_tangent(void);
				void compute_predictor(void);
				bool compute_corrector(void);
			};
		}
	}
}
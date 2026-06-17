#pragma once

//std
#include <cmath>
#include <climits>

//FEA
#include "FEA/inc/Analysis/Solvers/Eigen.hpp"
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class StaticNonlinear : public virtual Eigen, public virtual Nonlinear
			{
			private:
				//constructors
				StaticNonlinear(void);

				//destructor
				~StaticNonlinear(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				bool load_adjust(bool);
				bool load_adjust(void) const;

				bool stability(bool);
				bool stability(void) const;

				bool frequencies(bool);
				bool frequencies(void) const;

				//type
				Type type(void) const override;

				//sets
				uint32_t state_set(void) const override;
				uint32_t force_set(void) const override;
				uint32_t tangent_set(void) const override;

			private:
				//analysis
				void setup(void) override;
				void print(void) override;
				bool solve(void) override;

				//solve
				void solve_setup(void);
				void solve_restore(void);
				bool solve_predictor(void);
				bool solve_corrector(void);

				//compute
				bool compute_predictor(void);
				bool compute_corrector(void);
				bool compute_stability(void);
				bool compute_frequencies(void);
				void compute_load_factor(void);

				//data
				bool m_stability;
				bool m_frequencies;
				bool m_load_adjust;
				double m_load_factor;

				//friends
				friend class Solver;
			};
		}
	}
}
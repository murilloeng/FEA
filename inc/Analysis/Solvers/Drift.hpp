#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Field.hpp"
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Drift : public Nonlinear
			{
			private:
				//constructors
				Drift(void);

				//destructor
				~Drift(void) override;

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

				//data
				double scale(double);
				double scale(void) const;
				double threshold(double);
				double threshold(void) const;
				solvers::field field(void) const;
				solvers::field field(solvers::field);

			private:
				//analysis
				void setup(void) override;
				bool solve(void) override;

				void solve_force(void);
				void solve_stiffness(void);

				void random_state(void);

				static void drift_force(double*, const double*, void**);
				static void drift_energy(double*, const double*, void**);

				//data
				double m_scale;
				double m_threshold;
				solvers::field m_field;

				//friends
				friend class Solver;
			};
		}
	}
}
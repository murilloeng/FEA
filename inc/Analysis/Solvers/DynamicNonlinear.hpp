#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Dynamic.hpp"
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class DynamicNonlinear : public virtual Dynamic, public virtual Nonlinear
			{
			private:
				//constructors
				DynamicNonlinear(void);

				//destructor
				~DynamicNonlinear(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//type
				Type type(void) const override;

			private:
				//analysis
				void setup(void) override;

				void print_step(void) const;

				bool solve_newmark(void);
				bool solve_runge_kutta(void);

				void compute_time(void);
				void compute_update(void);
				void compute_tangent(void);
				void compute_restore(void);
				void compute_tangent_1(void);
				bool compute_tangent_2(void);
				bool compute_tangent_3(void);
				bool compute_tangent_4(void);
				void compute_predictor(void);
				bool compute_corrector(void);

				//friends
				friend class analysis::solvers::Solver;
			};
		}
	}
}
#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		class TangentDrift : public Solver
		{
		private:
			//constructor
			TangentDrift(void);

			//destructor
			~TangentDrift(void);

		public:
			//solve
			void solve(void) override;

			//data
			uint32_t tests(uint32_t);
			uint32_t tests(void) const;

			double dof_min(double);
			double dof_min(void) const;

			double dof_max(double);
			double dof_max(void) const;

			double tolerance(double);
			double tolerance(void) const;

			//data
			uint32_t state_set(void) const override;
			uint32_t force_set(void) const override;
			uint32_t tangent_set(void) const override;

		private:
			//compute
			void compute_state(void);

			//data
			uint32_t m_tests;
			double m_dof_min;
			double m_dof_max;
			double m_tolerance;

			//friends
			friend class fea::analysis::Analysis;
		};
	}
}
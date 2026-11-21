#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Incremental.hpp"

namespace math
{
	class Sparse;
}

namespace fea
{
	namespace analysis
	{
		class DynamicLinear : virtual public Incremental
		{
		private:
			//constructor
			DynamicLinear(void);

			//destructor
			~DynamicLinear(void);

		public:
			//solve
			void solve(void) override;
			void compute_step(void) override;

			//data
			uint32_t states_set(void) const override;
			uint32_t forces_set(void) const override;
			uint32_t energies_set(void) const override;
			uint32_t tangents_set(void) const override;

			double beta(double);
			double beta(void) const;

			double gamma(double);
			double gamma(void) const;

		private:
			//analysis
			void check(void) override;
			void setup(void) override;
			void print(void) override;

			//compute
			void compute_loads(void);
			void compute_energy(void);

			//data
			double m_g, m_b;
			math::Sparse* m_S;

			//friends
			friend class fea::analysis::Analysis;
		};
	}
}
#pragma once

//Math
#include "Math/inc/Solvers/NewtonRaphson.hpp"

//FEA
#include "FEA/inc/Analysis/Solvers/Incremental.hpp"

namespace fea
{
	namespace analysis
	{
		class StaticNonlinear : virtual public Incremental, virtual public math::solvers::NewtonRaphson
		{
		private:
			//constructor
			StaticNonlinear(void);

			//destructor
			~StaticNonlinear(void);

		public:
			//data
			bool stability(bool);
			bool stability(void) const;

			bool stability_data(uint32_t) const;

			//save
			void save(const char*, std::vector<std::pair<uint32_t, mesh::nodes::DOF>>) const override;

		private:
			//analysis
			void check(void) override;
			void setup(void) override;
			void print(void) override;

			//solve
			void solve(void) override;

			//compute
			void compute_stability(void);

			//data
			bool m_stability;
			bool* m_stability_data;

			//friends
			friend class fea::analysis::Analysis;
		};
	}
}
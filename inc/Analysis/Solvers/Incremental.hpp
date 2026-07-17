#pragma once

//std
#include <vector>
#include <cstdint>

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

//Math
#include "Math/inc/Solvers/Incremental.hpp"

namespace fea
{
	namespace analysis
	{
		class Incremental : virtual public Solver, virtual public math::solvers::Incremental
		{
		public:
			//constructor
			Incremental(void);

			//destructor
			~Incremental(void);

			//save
			using math::solvers::Solver::save;
			using math::solvers::Incremental::save;
			virtual void save(const char*, std::vector<std::pair<uint32_t, mesh::nodes::DOF>>) const;

		protected:
			//analysis
			void setup(void) override;
		};
	}
}
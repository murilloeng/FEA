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

			//draw
			uint32_t draw_steps(void) const override;
			void draw_positions(float*, float) const override;
			void draw_rotations(float*, float) const override;

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
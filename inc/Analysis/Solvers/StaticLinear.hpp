#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		class StaticLinear : public Solver
		{
		private:
			//constructor
			StaticLinear(void) = default;

			//destructor
			~StaticLinear(void) = default;

		public:
			//solve
			void solve(void) override;

			//data
			uint32_t states_set(void) const override;
			uint32_t forces_set(void) const override;
			uint32_t energies_set(void) const override;
			uint32_t tangents_set(void) const override;

			//draw
			uint32_t draw_steps(void) const override;
			void draw_positions(float*, float) const override;
			void draw_rotations(float*, float) const override;

			//friends
			friend class fea::analysis::Analysis;
		};
	}
}
//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticLinear.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		StaticLinear::StaticLinear(void)
		{
			return;
		}

		//destructor
		StaticLinear::~StaticLinear(void)
		{
			return;
		}

		//solve
		void StaticLinear::solve(void)
		{
			//assemble
			model_compute();
			m_analysis->assembler()->assemble_stiffness(m_K);
			m_analysis->assembler()->assemble_reference_force(m_fe);
			//solve
			math::solvers::Solver::solve(m_K, m_fe, m_x_new);
		}

		//data
		uint32_t StaticLinear::state_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::State::x);
		}
		uint32_t StaticLinear::force_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::Force::fe);
		}
		uint32_t StaticLinear::tangent_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::Tangent::K);
		}

		//draw
		uint32_t StaticLinear::draw_steps(void) const
		{
			return 2;
		}
		void StaticLinear::draw_positions(float*, float) const
		{
			return;
		}
		void StaticLinear::draw_rotations(float*, float) const
		{
			return;
		}
	}
}
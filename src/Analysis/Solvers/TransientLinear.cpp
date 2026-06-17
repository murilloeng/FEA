//FEA
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/TransientLinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			TransientLinear::TransientLinear(void)
			{
				return;
			}

			//destructor
			TransientLinear::~TransientLinear(void)
			{
				return;
			}

			//type
			Type TransientLinear::type(void) const
			{
				return Type::TransientLinear;
			}

			//sets
			uint32_t TransientLinear::state_set(void) const
			{
				return (uint32_t) solvers::state::x | (uint32_t) solvers::state::v;
			}
			uint32_t TransientLinear::force_set(void) const
			{
				return (uint32_t) solvers::force::fe | (uint32_t) solvers::force::R;
			}
			uint32_t TransientLinear::tangent_set(void) const
			{
				return (uint32_t) solvers::tangent::K | (uint32_t) solvers::tangent::C;
			}

			//analysis
			bool TransientLinear::solve(void)
			{
				return true;
			}

			void TransientLinear::record(void)
			{
				return;
			}
		}
	}
}
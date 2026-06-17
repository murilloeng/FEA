//FEA
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/TransientNonlinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			TransientNonlinear::TransientNonlinear(void)
			{
				return;
			}

			//destructor
			TransientNonlinear::~TransientNonlinear(void)
			{
				return;
			}

			//serialization
			void TransientNonlinear::load(FILE* file)
			{
				Solver::load(file);
				Time::load(file);
				Nonlinear::load(file);
			}
			void TransientNonlinear::save(FILE* file) const
			{
				Solver::save(file);
				Time::save(file);
				Nonlinear::save(file);
			}

			//type
			Type TransientNonlinear::type(void) const
			{
				return Type::TransientNonlinear;
			}

			//sets
			uint32_t TransientNonlinear::state_set(void) const
			{
				return 0;
			}
			uint32_t TransientNonlinear::force_set(void) const
			{
				return 0;
			}
			uint32_t TransientNonlinear::tangent_set(void) const
			{
				return 0;
			}

			//analysis
			bool TransientNonlinear::solve(void)
			{
				return true;
			}
			void TransientNonlinear::record(void)
			{
				return;
			}
		}
	}
}
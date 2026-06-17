#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Solvers/Convergence.hpp"
#include "FEA/inc/Analysis/Solvers/Continuation.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Nonlinear : public virtual Solver
			{
			protected:
				//constructors
				Nonlinear(void);

				//destructor
				virtual ~Nonlinear(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				uint32_t attempt_max(uint32_t);
				uint32_t attempt_max(void) const;

				uint32_t iteration_max(uint32_t);
				uint32_t iteration_max(void) const;

				uint32_t iteration_desired(uint32_t);
				uint32_t iteration_desired(void) const;

				Convergence& convergence(void);
				const Convergence& convergence(void) const;
				Convergence& convergence(Convergence::Type);

				Continuation& continuation(void);
				const Continuation& continuation(void) const;
				Continuation& continuation(Continuation::Type);

			protected:
				//analysis
				void setup(void) override;

				//data
				uint32_t m_attempt;
				uint32_t m_attempt_max;

				uint32_t m_iteration;
				uint32_t m_iteration_max;
				uint32_t m_iteration_desired;

				Convergence m_convergence;
				Continuation m_continuation;

				//friends
				friend class Convergence;
				friend class Continuation;
				friend class StopCriteria;
			};
		}
	}
}
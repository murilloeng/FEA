#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Time.hpp"
#include "FEA/inc/Analysis/Solvers/Newmark.hpp"
#include "FEA/inc/Analysis/Solvers/Integration.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Dynamic : public Time
			{
			protected:
				//constructors
				Dynamic(void);

				//destructor
				~Dynamic(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//sets
				uint32_t state_set(void) const override;
				uint32_t force_set(void) const override;
				uint32_t tangent_set(void) const override;

				//data
				solvers::Newmark& newmark(void);

				solvers::Integration integration(void) const;
				solvers::Integration integration(solvers::Integration);

			protected:
				//analysis
				bool solve(void) override;

				bool compute_initial(void);
				bool compute_acceleration(void);

				virtual bool solve_newmark(void) = 0;
				virtual bool solve_runge_kutta(void) = 0;

				//data
				solvers::Newmark m_newmark;
				solvers::Integration m_integration;
			};
		}
	}
}
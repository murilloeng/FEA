#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Time : public virtual Solver
			{
			protected:
				//constructors
				Time(void);

				//destructor
				virtual ~Time(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				bool time_adjust(bool);
				bool time_adjust(void) const;

			protected:
				//data
				bool m_time_adjust;
			};
		}
	}
}
#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Time.hpp"
#include "FEA/inc/Analysis/Solvers/Euller.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Transient : public virtual Time
			{
			protected:
				//constructors
				Transient(void);

				//destructor
				virtual ~Transient(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				solvers::Euller& euller(void);

			protected:
				//data
				solvers::Euller m_euller;
			};
		}
	}
}
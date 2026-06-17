#pragma once

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Solvers/Spectre.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Eigen : public virtual Solver
			{
			protected:
				//constructors
				Eigen(void);

				//destructor
				virtual ~Eigen(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				double scale(double);
				double scale(void) const;

				uint32_t spectre_min(uint32_t);
				uint32_t spectre_max(uint32_t);
				uint32_t spectre_min(void) const;
				uint32_t spectre_max(void) const;

				solvers::spectre spectre(void) const;
				solvers::spectre spectre(solvers::spectre);

			protected:
				//solve
				bool eigen_std(void) const;
				bool eigen_gen(void) const;
				uint32_t modes_count(void) const;

				//data
				double m_scale;
				uint32_t m_mode_crop;
				uint32_t m_spectre_min;
				uint32_t m_spectre_max;
				solvers::spectre m_spectre;
			};
		}
	}
}
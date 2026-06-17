#pragma once

//std
#include <cstdio>

namespace fea
{
	namespace boundary
	{
		class Baumgarte
		{
		public:
			//constructors
			Baumgarte(void);

			//destructor
			~Baumgarte(void);

			//serialization
			void load(FILE*);
			void save(FILE*) const;

			//data
			double beta(double);
			double beta(void) const;

			double alpha(double);
			double alpha(void) const;

		private:
			//data
			double m_beta;
			double m_alpha;
		};
	}
}
//std
#include <vector>
#include <cstdint>

//FEA
#include "FEA/inc/Boundary/Time/Time.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			class Linear : public Time
			{
			private:
				//constructors
				Linear(void);

				//destructor
				~Linear(void);

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				bool pulse(bool);
				bool pulse(void) const;

				bool mirror(bool);
				bool mirror(void) const;

				bool periodic(bool);
				bool periodic(void) const;

				double period(double);
				double period(void) const;

				double term(uint32_t) const;
				double term(uint32_t, double);

				//type
				time::Type type(void) const override;

				//value
				double value(double) const override;

			private:
				//data
				bool m_pulse;
				bool m_mirror;
				bool m_periodic;
				double m_period;
				double m_terms[2];

				//friends
				friend class Time;
			};
		}
	}
}
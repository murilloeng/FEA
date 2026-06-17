//std
#include <vector>

//FEA
#include "FEA/inc/Boundary/Time/Time.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			class Quadratic : public Time
			{
			private:
				//constructors
				Quadratic(void);

				//destructor
				~Quadratic(void);

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				bool pulse(bool);
				bool pulse(void) const;

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
				bool m_periodic;
				double m_period;
				double m_terms[3];

				//private
				friend class Time;
			};
		}
	}
}
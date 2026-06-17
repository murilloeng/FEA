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
			class Constant : public Time
			{
			private:
				//constructors
				Constant(void);

				//destructor
				~Constant(void);

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				bool pulse(bool);
				bool pulse(void) const;

				double value(double);
				double value(void) const;

				double period(double);
				double period(void) const;

				//type
				time::Type type(void) const override;

				//value
				double value(double) const override;

			private:
				//data
				bool m_pulse;
				double m_value;
				double m_period;

				//friends
				friend class Time;
			};
		}
	}
}
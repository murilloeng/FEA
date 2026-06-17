//FEA
#include "FEA/inc/Boundary/Time/Time.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			class SineWave : public Time
			{
			private:
				//constructors
				SineWave(void);

				//destructor
				~SineWave(void);

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				bool pulse(bool);
				bool pulse(void) const;

				double phase(double);
				double phase(void) const;

				double period(double);
				double period(void) const;

				double amplitude(double);
				double amplitude(void) const;

				double frequency(double);
				double frequency(void) const;

				double angular_frequency(double);
				double angular_frequency(void) const;

				//type
				time::Type type(void) const override;

				//value
				double value(double) const override;

			private:
				//data
				bool m_pulse;
				double m_phase;
				double m_period;
				double m_amplitude;

				//friends
				friend class Time;
			};
		}
	}
}
//std
#include <cmath>
#include <stdexcept>

//FEA
#include "FEA/inc/Boundary/Time/Types.hpp"
#include "FEA/inc/Boundary/Time/SineWave.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			//constructors
			SineWave::SineWave(void) : m_pulse(false), m_phase(0), m_period(1), m_amplitude(1)
			{
				return;
			}

			//destructor
			SineWave::~SineWave(void)
			{
				return;
			}

			//serialization
			void SineWave::load(FILE* file)
			{
				uint32_t pulse;
				Time::load(file);
				if(fscanf(file, "%d %lf %lf %lf", &pulse, &m_phase, &m_period, &m_amplitude) != 4)
				{
					throw std::runtime_error("Sine wave time loading error!");
				}
				m_pulse = pulse;
			}
			void SineWave::save(FILE* file) const
			{
				Time::save(file);
				fprintf(file, "%d %+.6e %+.6e %+.6e", m_pulse, m_phase, m_period, m_amplitude);
			}

			//data
			bool SineWave::pulse(bool pulse)
			{
				return m_pulse = pulse;
			}
			bool SineWave::pulse(void) const
			{
				return m_pulse;
			}

			double SineWave::phase(void) const
			{
				return m_phase;
			}
			double SineWave::phase(double phase)
			{
				return m_phase = phase;
			}

			double SineWave::period(void) const
			{
				return m_period;
			}
			double SineWave::period(double period)
			{
				return m_period = period;
			}

			double SineWave::amplitude(void) const
			{
				return m_amplitude;
			}
			double SineWave::amplitude(double amplitude)
			{
				return m_amplitude = amplitude;
			}

			double SineWave::frequency(double f)
			{
				m_period = 1 / f;
				return f;
			}
			double SineWave::frequency(void) const
			{
				return 1 / m_period;
			}

			double SineWave::angular_frequency(double w)
			{
				m_period = 2 * M_PI / w;
				return w;
			}
			double SineWave::angular_frequency(void) const
			{
				return 2 * M_PI / m_period;
			}

			//type
			time::Type SineWave::type(void) const
			{
				return time::Type::SineWave;
			}

			//value
			double SineWave::value(double t) const
			{
				const double a = m_phase;
				const double T = m_period;
				const double A = m_amplitude;
				return (!m_pulse || t < T) * A * sin(2 * M_PI * t / T + a);
			}
		}
	}
}
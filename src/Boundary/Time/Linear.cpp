//std
#include <cmath>
#include <cstring>
#include <stdexcept>

//FEA
#include "FEA/inc/Boundary/Time/Types.hpp"
#include "FEA/inc/Boundary/Time/Linear.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			//constructors
			Linear::Linear(void) : m_pulse{false}, m_mirror{false}, m_periodic{false}, m_period(1), m_terms{0, 0}
			{
				return;
			}

			//destructor
			Linear::~Linear(void)
			{
				return;
			}

			//serialization
			void Linear::load(FILE* file)
			{
				//data
				Time::load(file);
				uint32_t pulse, mirror, periodic;
				if(fscanf(file, "%d %d %d %lf %lf %lf", &pulse, &mirror, &periodic, &m_period, &m_terms[0], &m_terms[1]) != 6)
				{
					throw std::runtime_error("Linear time loading error!");
				}
				m_pulse = pulse;
				m_mirror = mirror;
				m_periodic = periodic;
			}
			void Linear::save(FILE* file) const
			{
				Time::save(file);
				if(fprintf(file, "%d %d %d %+.6e %+.6e %+.6e ", m_pulse, m_mirror, m_periodic, m_period, m_terms[0], m_terms[1]) < 0)
				{
					throw std::runtime_error("Linear time saving error!");
				}
			}

			//data
			bool Linear::pulse(void) const
			{
				return m_pulse;
			}
			bool Linear::pulse(bool pulse)
			{
				return m_pulse = pulse;
			}

			bool Linear::mirror(void) const
			{
				return m_mirror;
			}
			bool Linear::mirror(bool mirror)
			{
				return m_mirror = mirror;
			}

			bool Linear::periodic(void) const
			{
				return m_periodic;
			}
			bool Linear::periodic(bool periodic)
			{
				return m_periodic = periodic;
			}

			double Linear::period(void) const
			{
				return m_period;
			}
			double Linear::period(double period)
			{
				return m_period = period;
			}

			double Linear::term(uint32_t index) const
			{
				return m_terms[index];
			}
			double Linear::term(uint32_t index, double term)
			{
				return m_terms[index] = term;
			}

			//type
			time::Type Linear::type(void) const
			{
				return time::Type::Linear;
			}

			//value
			double Linear::value(double t) const
			{
				//setup
				if(m_pulse && t > m_period) return 0;
				if(m_periodic || m_mirror) t = fmod(t, m_period);
				if(m_mirror && t > m_period / 2) t = m_period - t;
				//return
				return m_terms[0] + t * m_terms[1];
			}
		}
	}
}
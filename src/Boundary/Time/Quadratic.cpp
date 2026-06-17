//std
#include <cmath>
#include <cstring>
#include <stdexcept>

//FEA
#include "FEA/inc/Boundary/Time/Types.hpp"
#include "FEA/inc/Boundary/Time/Quadratic.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			//constructors
			Quadratic::Quadratic(void) : m_pulse(false), m_periodic(false), m_period(1), m_terms{0, 0, 0}
			{
				return;
			}

			//destructor
			Quadratic::~Quadratic(void)
			{
				return;
			}

			//serialization
			void Quadratic::load(FILE* file)
			{
				//data
				Time::load(file);
				uint32_t pulse, periodic;
				if(fscanf(file, "%d %d %lf %lf %lf %lf", &pulse, &periodic, &m_period, &m_terms[0], &m_terms[1], &m_terms[2]) != 6)
				{
					throw std::runtime_error("Quadratic time loading error!");
				}
				m_periodic = periodic;
			}
			void Quadratic::save(FILE* file) const
			{
				Time::save(file);
				fprintf(file, "%d %d %+.6e %+.6e %+.6e %+.6e ", m_pulse, m_periodic, m_period, m_terms[0], m_terms[1], m_terms[2]);
			}

			//data
			bool Quadratic::pulse(void) const
			{
				return m_pulse;
			}
			bool Quadratic::pulse(bool pulse)
			{
				return m_pulse = pulse;
			}

			bool Quadratic::periodic(void) const
			{
				return m_periodic;
			}
			bool Quadratic::periodic(bool periodic)
			{
				return m_periodic = periodic;
			}

			double Quadratic::period(void) const
			{
				return m_period;
			}
			double Quadratic::period(double period)
			{
				return m_period = period;
			}

			double Quadratic::term(uint32_t index) const
			{
				return m_terms[index];
			}
			double Quadratic::term(uint32_t index, double term)
			{
				return m_terms[index] = term;
			}

			//type
			time::Type Quadratic::type(void) const
			{
				return time::Type::Quadratic;
			}

			//value
			double Quadratic::value(double t) const
			{
				t = (m_periodic && !m_pulse) ? fmod(t, m_period) : t;
				return (!m_pulse || t < m_period) * (m_terms[0] + t * m_terms[1] + t * t * m_terms[2]);
			}
		}
	}
}
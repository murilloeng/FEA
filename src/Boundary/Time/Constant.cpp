//std
#include <cmath>
#include <cstring>
#include <stdexcept>

//FEA
#include "FEA/inc/Boundary/Time/Types.hpp"
#include "FEA/inc/Boundary/Time/Constant.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			//constructors
			Constant::Constant(void) : m_pulse{false}, m_value{0}, m_period{1}
			{
				return;
			}

			//destructor
			Constant::~Constant(void)
			{
				return;
			}

			//serialization
			void Constant::load(FILE* file)
			{
				//data
				uint32_t pulse;
				Time::load(file);
				if(fscanf(file, "%d %lf %lf", &pulse, &m_period, &m_value) != 3)
				{
					throw std::runtime_error("Constant time loading error!");
				}
				m_pulse = pulse;
			}
			void Constant::save(FILE* file) const
			{
				Time::save(file);
				if(fprintf(file, "%d %+.6e %+.6e ", m_pulse, m_period, m_value) < 0)
				{
					throw std::runtime_error("Constant time saving error!");
				}
			}

			//data
			bool Constant::pulse(void) const
			{
				return m_pulse;
			}
			bool Constant::pulse(bool pulse)
			{
				return m_pulse = pulse;
			}

			double Constant::period(void) const
			{
				return m_period;
			}
			double Constant::period(double period)
			{
				return m_period = period;
			}

			double Constant::value(void) const
			{
				return m_value;
			}
			double Constant::value(double value)
			{
				return m_value = value;
			}

			//type
			time::Type Constant::type(void) const
			{
				return time::Type::Constant;
			}

			//value
			double Constant::value(double t) const
			{
				return m_pulse && t > m_period ? 0 : m_value;
			}
		}
	}
}
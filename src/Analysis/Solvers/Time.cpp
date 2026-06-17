//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Time.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Time::Time(void) : m_time_adjust(false)
			{
				return;
			}

			//destructor
			Time::~Time(void)
			{
				return;
			}

			//serialization
			void Time::load(FILE* file)
			{
				uint32_t time_adjust;
				if(fscanf(file, "%d", &time_adjust) != 1)
				{
					throw std::runtime_error("Unable to read Time solver data from file!");
				}
				m_time_adjust = time_adjust;
			}
			void Time::save(FILE* file) const
			{
				if(fprintf(file, "%d ", m_time_adjust) < 0)
				{
					throw std::runtime_error("Unable to write Time solver data to file!");
				}
			}

			//data
			bool Time::time_adjust(void) const
			{
				return m_time_adjust;
			}
			bool Time::time_adjust(bool time_adjust)
			{
				return m_time_adjust = time_adjust;
			}
		}
	}
}
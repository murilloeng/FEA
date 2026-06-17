//std
#include <cmath>

//FEA
#include "FEA/inc/Boundary/Time/Types.hpp"
#include "FEA/inc/Boundary/Time/Custom.hpp"

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			//constructors
			Custom::Custom(void) : m_function([](double) { return 0; })
			{
				return;
			}

			//destructor
			Custom::~Custom(void)
			{
				return;
			}

			//serialization
			void Custom::load(FILE* file)
			{
				Time::load(file);
			}
			void Custom::save(FILE* file) const
			{
				Time::save(file);
			}

			//data
			std::function<double(double)> Custom::function(void) const
			{
				return m_function;
			}
			std::function<double(double)> Custom::function(std::function<double(double)> function)
			{
				return m_function = function;
			}

			//type
			time::Type Custom::type(void) const
			{
				return time::Type::Custom;
			}

			//value
			double Custom::value(double t) const
			{
				return m_function(t);
			}
		}
	}
}
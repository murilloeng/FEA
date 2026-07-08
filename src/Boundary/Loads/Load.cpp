//FEA
#include "FEA/inc/Boundary/Loads/Load.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructor
			Load::Load(void) : m_value{0}
			{
				return;
			}
		
			//destructor
			Load::~Load(void)
			{
				return;
			}

			//data
			double Load::value(void) const
			{
				return m_value;
			}
			double Load::value(double value)
			{
				return m_value = value;
			}

			std::function<double(double)> Load::function(void) const
			{
				return m_function;
			}
			std::function<double(double)> Load::function(std::function<double(double)> function)
			{
				return m_function = function;
			}

			//analysis
			void Load::check(void)
			{
				return;
			}
			void Load::setup(void)
			{
				return;
			}

			//static
			Boundary* Load::m_boundary = nullptr;
		}
	}
}
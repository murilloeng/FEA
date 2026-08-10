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

			//serialization
			void Load::save(FILE* file) const
			{
				fprintf(file, "Value: %+.6e ", m_value);
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

			double Load::time_function(double t) const
			{
				return m_time_function ? m_time_function(t) : 1;
			}
			Load::Function Load::time_function(void) const
			{
				return m_time_function;
			}
			Load::Function Load::time_function(Load::Function time_function)
			{
				return m_time_function = time_function;
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
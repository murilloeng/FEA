//std
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Frame.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructors
			Frame::Frame(void) : m_section{nullptr}
			{
				return;
			}

			//destructor
			Frame::~Frame(void)
			{
				return;
			}

			//data
			const sections::Section* Frame::section(void) const
			{
				return m_section;
			}
			const sections::Section* Frame::section(const sections::Section* section)
			{
				return m_section = section;
			}

			//analysis
			void Frame::check(void)
			{
				Mechanic::check();
				if(!m_section) throw std::runtime_error("Error: Frame element with unset section!");
			}
		}
	}
}
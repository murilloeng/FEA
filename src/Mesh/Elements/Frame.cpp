//std
#include <stdexcept>

//FEA
#include "FEA/inc/Mesh/Elements/Frame.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
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
				if(!m_section)
				{
					throw std::runtime_error("Error: Truss element with unset section!");
				}
			}
		}
	}
}
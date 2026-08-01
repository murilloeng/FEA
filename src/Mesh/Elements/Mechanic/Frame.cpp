//std
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Frame.hpp"

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
			uint32_t Frame::dimension(void) const
			{
				return 1;
			}

			const sections::Section* Frame::section(void) const
			{
				return m_section;
			}
			const sections::Section* Frame::section(sections::Section* section)
			{
				return m_section = section;
			}

			//analysis
			void Frame::check(void)
			{
				if(!m_section)
				{
					throw std::runtime_error("Error: Frame element has unset section!");
				}
				Mechanic::check();
			}
			void Frame::setup(void)
			{
				//data
				Mechanic::setup();
				const math::Vec3 z1 = node(0)->position_ref();
				const math::Vec3 z2 = node(1)->position_ref();
				//length
				m_Lr = (z2 - z1).norm();
			}
		}
	}
}
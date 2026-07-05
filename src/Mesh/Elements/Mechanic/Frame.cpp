//std
#include <stdexcept>

//FEA
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

			//analysis
			void Frame::check(void)
			{
				if(!m_section)
				{
					throw std::runtime_error("Error: Frame element has unset section!");
				}
				Mechanic::check();
			}
		}
	}
}
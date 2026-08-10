//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Section.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Section::Section(const sections::Section* section)
			{
				m_fibers.reserve(section->fibers().size());
				for(uint32_t i = 0; i < section->fibers().size(); i++)
				{
					m_fibers.push_back(Fiber(section->fiber(i)));
				}
			}

			//destructor
			Section::~Section(void)
			{
				return;
			}

			//data
			Fiber& Section::fiber(uint32_t index)
			{
				return m_fibers[index];
			}
			const Fiber& Section::fiber(uint32_t index) const
			{
				return m_fibers[index];
			}

			std::vector<Fiber>& Section::fibers(void)
			{
				return m_fibers;
			}
			const std::vector<Fiber>& Section::fibers(void) const
			{
				return m_fibers;
			}
		}
	}
}
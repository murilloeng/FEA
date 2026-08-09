#pragma once

//std
#include <vector>

//sections
#include "Sections/inc/Section.hpp"

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Fiber.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Section
			{
			public:
				//constructor
				Section(const sections::Section*);

				//destructor
				~Section(void);

				//data
				Fiber& fiber(uint32_t);
				const Fiber& fiber(uint32_t) const;

				std::vector<Fiber>& fibers(void);
				const std::vector<Fiber>& fibers(void) const;

			private:
				//data
				std::vector<Fiber> m_fibers;
			};
		}
	}
}
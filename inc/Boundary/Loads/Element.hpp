#pragma once

//std
#include <vector>
#include <cstdint>

//FEA
#include "FEA/inc/Boundary/Loads/Load.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Element : public Load
			{
			public:
				//constructor
				Element(void);

				//destructor
				~Element(void);

				//data
				std::vector<uint32_t> m_elements;
			};
		}
	}
}
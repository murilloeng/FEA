#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Element
			{
			public:
				//constructor
				Element(void);

				//destructor
				~Element(void);

				//data
				std::vector<uint32_t> m_nodes;
			};
		}
	}
}
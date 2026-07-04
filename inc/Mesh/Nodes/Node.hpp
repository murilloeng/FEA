#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node
			{
			public:
				//constructor
				Node(void);

				//destructor
				~Node(void);

				//data
				uint32_t m_dof;
				double m_position[3];
				std::vector<uint32_t> m_dof_indexes;
			};
		}
	}
}
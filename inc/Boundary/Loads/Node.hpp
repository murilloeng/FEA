#pragma once

//std
#include "cstdint"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Boundary/Loads/Load.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Node : public Load
			{
			public:
				//constructor
				Node(void);

				//destructor
				~Node(void);

				//data
				uint32_t m_node;
				mesh::nodes::DOF m_dof;
			};
		}
	}
}
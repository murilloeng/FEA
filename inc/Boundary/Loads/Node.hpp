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

				//analysis
				void check(void);
				void setup(void);

				//data
				uint32_t m_node;
				uint32_t m_dof_index;
				mesh::nodes::DOF m_dof;
			};
		}
	}
}
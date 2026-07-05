#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			enum class DOF : uint32_t;
		}
	}
	namespace boundary
	{
		class Boundary;
	}
}

namespace fea
{
	namespace boundary
	{
		class Support
		{
		public:
			//constructor
			Support(void);

			//destructor
			~Support(void);

			//data
			uint32_t m_node;
			uint32_t m_dof_index;
			mesh::nodes::DOF m_dof;
			static Boundary* m_boundary;
		};
	}
}
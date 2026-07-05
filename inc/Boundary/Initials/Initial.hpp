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
		class Initial
		{
		public:
			//constructor
			Initial(void);

			//destructor
			~Initial(void);

			//data
			double m_state;
			double m_velocity;

			uint32_t m_node;
			mesh::nodes::DOF m_dof;

			static Boundary* m_boundary;
		};
	}
}
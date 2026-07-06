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
		class Dependency
		{
		public:
			//constructor
			Dependency(void);

			//destructor
			~Dependency(void);

			//analysis
			void check(void);
			void setup(void);
			uint32_t dof_index(bool) const;

			//check
			void check_inner(void) const;
			void check_outer(void) const;
			void check_nodes(void) const;
			void check_initials(void) const;
			void check_supports(void) const;

			//operators
			bool operator==(const Dependency&) const;

			//data
			uint32_t m_nodes[2];
			mesh::nodes::DOF m_dof[2];
			static Boundary* m_boundary;
		};
	}
}
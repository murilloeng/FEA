#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node;
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
			uint32_t index(void) const;

			static Boundary* boundary(void);
			
			mesh::nodes::Node* node(uint32_t);
			mesh::nodes::Node* node(void) const;

			mesh::nodes::DOF dof(void) const;
			mesh::nodes::DOF dof(mesh::nodes::DOF);

		private:
			//data
			uint32_t m_node;
			uint32_t m_index;
			mesh::nodes::DOF m_dof;
			static Boundary* m_boundary;
		};
	}
}
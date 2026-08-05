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
	namespace analysis
	{
		class Assembler;
	}
}

namespace fea
{
	namespace boundary
	{
		class Dependency
		{
		private:
			//constructor
			Dependency(void);

			//destructor
			~Dependency(void);

			//serialization
			void save(FILE*) const;

		public:
			//data
			mesh::nodes::Node* node(uint32_t) const;
			mesh::nodes::Node* node(uint32_t, uint32_t);

			mesh::nodes::DOF dof(uint32_t) const;
			mesh::nodes::DOF dof(uint32_t, mesh::nodes::DOF);

			//index
			uint32_t index(void) const;

			uint32_t index_node(uint32_t) const;
			uint32_t index_node(uint32_t, uint32_t);

		private:
			//analysis
			void check(void);
			void setup(void);

			//check
			void check_inner(void) const;
			void check_outer(void) const;
			void check_nodes(void) const;
			void check_initials(void) const;
			void check_supports(void) const;

			//operators
			bool operator==(const Dependency&) const;

			//data
			uint32_t m_index;
			uint32_t m_nodes[2];
			mesh::nodes::DOF m_dof[2];
			static Boundary* m_boundary;

			//friends
			friend class fea::boundary::Boundary;
			friend class fea::analysis::Assembler;
		};
	}
}
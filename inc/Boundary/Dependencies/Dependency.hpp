#pragma once

//std
#include <list>
#include <vector>
#include <functional>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			enum class dof : uint32_t;
		}
		namespace joints
		{
			class Joint;
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
			//constructors
			Dependency(uint32_t, mesh::nodes::dof, uint32_t, mesh::nodes::dof);

			//destructor
			~Dependency(void);

			//serialization
			void load(FILE*);
			void save(FILE*) const;

		public:
			//data
			static Boundary* boundary(void);

			mesh::nodes::Node* node(uint32_t) const;
			mesh::nodes::Node* node(uint32_t, uint32_t);

			mesh::nodes::dof dof(uint32_t) const;
			mesh::nodes::dof dof(uint32_t, mesh::nodes::dof);

			//index
			uint32_t index(void) const;
			uint32_t index_node(uint32_t) const;

		private:
			//analysis
			void setup(void);
			bool check(void) const;
			void apply_dof(void) const;

			//check
			bool check_inner(void) const;
			bool check_outer(void) const;
			bool check_nodes(void) const;
			bool check_initials(void) const;
			bool check_supports(void) const;

			//dof
			uint32_t dof_index(bool) const;

			//operators
			bool operator==(const Dependency&) const;

			//data
			uint32_t m_index;
			uint32_t m_nodes[2];
			mesh::nodes::dof m_dof[2];
			static Boundary* m_boundary;

			//friends
			friend class boundary::Boundary;
			friend class analysis::Assembler;
			friend class mesh::joints::Joint;
		};
	}
}
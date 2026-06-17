#pragma once

//FEA
#include "FEA/inc/Boundary/Loads/Load.hpp"

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node;
			enum class dof : uint32_t;
		}
	}
	namespace boundary
	{
		class Boundary;
		namespace loads
		{
			class LoadCase;
		}
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
		namespace loads
		{
			class Node : public Load
			{
			protected:
				//constructors
				Node(LoadCase*);

				//destructor
				~Node(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				mesh::nodes::dof dof(void) const;
				mesh::nodes::dof dof(mesh::nodes::dof);

				mesh::nodes::Node* node(void) const;
				mesh::nodes::Node* node(uint32_t);

				//index
				uint32_t index(void) const;
				uint32_t index_node(void) const;

			protected:
				//analysis
				void apply_dof(void) const;
				void prepare(void) override;
				bool check(void) const override;

				//data
				uint32_t m_node;
				uint32_t m_index;
				uint32_t m_dof_index;
				mesh::nodes::dof m_dof;

				//friends
				friend class LoadCase;
				friend class boundary::Boundary;
				friend class analysis::Assembler;
			};
		}
	}
}
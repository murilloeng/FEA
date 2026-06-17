#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>
#include <climits>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			enum class dof : uint32_t;
		}
	}
	namespace boundary
	{
		class Boundary;
		namespace loads
		{
			class Node;
			class Element;
			enum class Type : uint32_t;
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
			class LoadCase
			{
			private:
				//constructors
				LoadCase(void);

				//destructor
				~LoadCase(void);

				//serialization
				void load(FILE*);
				void save(FILE*) const;

			public:
				//data
				const char* label(void) const;
				const char* label(const char*);

				static Boundary* boundary(void);

				loads::Node* load_node(uint32_t) const;
				loads::Element* load_element(uint32_t) const;

				//lists
				const std::vector<loads::Node*>& loads_nodes(void) const;
				const std::vector<loads::Element*>& loads_elements(void) const;

				//create
				void create_load_element(uint32_t, Type, double = 1, uint32_t = UINT_MAX);
				void create_load_node(uint32_t, mesh::nodes::dof, double = 1, uint32_t = UINT_MAX);

				//remove
				void remove_load_node(uint32_t);
				void remove_load_element(uint32_t);

				void remove_load_node(const Node*);
				void remove_load_element(const Element*);

				//index
				uint32_t index(void) const;

			private:
				//check
				bool check(void) const;

				//analysis
				void setup(void);
				void apply_dof(void) const;

				//data
				uint32_t m_index;
				char m_label[200];
				static Boundary* m_boundary;
				std::vector<loads::Node*> m_loads_nodes;
				std::vector<loads::Element*> m_loads_elements;

				//friends
				friend class boundary::Boundary;
				friend class analysis::Assembler;
			};
		}
	}
}
#pragma once

//std
#include <vector>

namespace fea
{
	namespace boundary
	{
		class Boundary;
		namespace loads
		{
			class Node;
			class Element;
		}
	}
}

namespace fea
{
	namespace boundary
	{
		class LoadCase
		{
		private:
			//constructor
			LoadCase(void);

			//destructor
			~LoadCase(void);

			//serialization
			void save(FILE*) const;

		public:
			//data
			static Boundary* boundary(void);

			loads::Node* load_node(uint32_t) const;
			loads::Element* load_element(uint32_t) const;

			const std::vector<loads::Node*>& loads_nodes(void) const;
			const std::vector<loads::Element*>& loads_elements(void) const;

			//create
			void create_load_node(uint32_t, mesh::nodes::DOF, double);

		private:
			//analysis
			void check(void);
			void setup(void);

			//data
			uint32_t m_index;
			static Boundary* m_boundary;
			std::vector<loads::Node*> m_loads_nodes;
			std::vector<loads::Element*> m_loads_elements;

			//friends
			friend class fea::boundary::Boundary;
		};
	}
}
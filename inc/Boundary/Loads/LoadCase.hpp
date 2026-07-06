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
		public:
			//constructor
			LoadCase(void);

			//destructor
			~LoadCase(void);

			//analysis
			void check(void);
			void setup(void);

			//data
			static Boundary* m_boundary;
			std::vector<loads::Node*> m_loads_nodes;
			std::vector<loads::Element*> m_loads_elements;
		};
	}
}
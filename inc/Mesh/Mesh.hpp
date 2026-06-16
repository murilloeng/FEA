#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node;
		}
		namespace elements
		{
			class Element;
		}
	}
}

namespace fea
{
	namespace mesh
	{
		class Mesh
		{
		public:
			//constructor
			Mesh(void);
	
			//destructor
			~Mesh(void);
	
			//data
			nodes::Node* node(uint32_t) const;
			const std::vector<nodes::Node*>& nodes(void) const;
	
			elements::Element* element(uint32_t) const;
			const std::vector<elements::Element*>& elements(void) const;
	
		private:
			//data
			std::vector<nodes::Node*> m_nodes;
			std::vector<elements::Element*> m_elements;
		};
	}
}
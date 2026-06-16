#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	class Node;
	class Element;
}

namespace fea
{
	class Mesh
	{
	public:
		//constructor
		Mesh(void);

		//destructor
		~Mesh(void);

		//data
		Node* node(uint32_t) const;
		const std::vector<Node*>& nodes(void) const;

		Element* element(uint32_t) const;
		const std::vector<Element*>& elements(void) const;

	private:
		//data
		std::vector<Node*> m_nodes;
		std::vector<Element*> m_elements;
	};
}
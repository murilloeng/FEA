//FEA
#include "FEA/inc/Mesh/Mesh.hpp"

namespace fea
{
	//constructor
	Mesh::Mesh(void)
	{
		return;
	}
	
	//destructor
	Mesh::~Mesh(void)
	{
		return;
	}

	//data
	Node* Mesh::node(uint32_t index) const
	{
		return m_nodes[index];
	}
	const std::vector<Node*>& Mesh::nodes(void) const
	{
		return m_nodes;
	}

	Element* Mesh::element(uint32_t index) const
	{
		return m_elements[index];
	}
	const std::vector<Element*>& Mesh::elements(void) const
	{
		return m_elements;
	}
}
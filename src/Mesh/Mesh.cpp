//FEA
#include "FEA/inc/Mesh/Mesh.hpp"

namespace fea
{
	namespace mesh
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
		nodes::Node* Mesh::node(uint32_t index) const
		{
			return m_nodes[index];
		}
		const std::vector<nodes::Node*>& Mesh::nodes(void) const
		{
			return m_nodes;
		}
	
		elements::Element* Mesh::element(uint32_t index) const
		{
			return m_elements[index];
		}
		const std::vector<elements::Element*>& Mesh::elements(void) const
		{
			return m_elements;
		}
	}
}
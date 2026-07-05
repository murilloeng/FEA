//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

namespace fea
{
	namespace mesh
	{
		//constructor
		Mesh::Mesh(void)
		{
			nodes::Node::m_mesh = this;
			elements::Element::m_mesh = this;
		}
		
		//destructor
		Mesh::~Mesh(void)
		{
			for(const nodes::Node* node : m_nodes) delete node;
			for(const elements::Element* element : m_elements) delete element;
		}

		//static
		Model* Mesh::m_model = nullptr;
	}
}
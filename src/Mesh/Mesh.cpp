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

		//analysis
		void Mesh::check(void)
		{
			for(elements::Element* element : m_elements) element->check();
		}
		void Mesh::setup(void)
		{
			for(nodes::Node* node : m_nodes) node->setup();
			for(elements::Element* element : m_elements) element->setup();
		}
		void Mesh::dof_apply(void)
		{
			for(const elements::Element* element : m_elements)
			{
				for(uint32_t i = 0; i < element->m_nodes.size(); i++)
				{
					m_nodes[element->m_nodes[i]]->m_dof_set |= element->dof_set(i);
				}
			}
		}
		void Mesh::dof_setup(uint32_t& dof_counter)
		{
			for(nodes::Node* node : m_nodes) node->dof_setup(dof_counter);
		}

		//static
		Model* Mesh::m_model = nullptr;
	}
}
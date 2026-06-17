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
		void Mesh::record(void)
		{
			for(nodes::Node* node : m_nodes) node->record();
			for(elements::Element* element : m_elements) element->record();
		}
		void Mesh::update(void)
		{
			for(nodes::Node* node : m_nodes) node->update();
			for(elements::Element* element : m_elements) element->restore();
		}
		void Mesh::restore(void)
		{
			for(nodes::Node* node : m_nodes) node->restore();
			for(elements::Element* element : m_elements) element->restore();
		}
		void Mesh::compute(void)
		{
			for(elements::Element* element : m_elements) element->compute();
		}

		//static data
		Model* Mesh::m_model = nullptr;
	}
}
//std
#include <functional>

//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss3D.hpp"

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

		//serialization
		void Mesh::save(FILE* file) const
		{
			//header
			fprintf(file, "# Mesh\n");
			//nodes
			fprintf(file, "\n");
			fprintf(file, "## Nodes: %zd\n", m_nodes.size());
			for(const nodes::Node* node : m_nodes) node->save(file), fprintf(file, "\n");
			//elements
			fprintf(file, "\n");
			fprintf(file, "## Elements: %zd\n", m_elements.size());
			for(const elements::Element* element : m_elements) element->save(file), fprintf(file, "\n");
		}

		//data
		Model* Mesh::model(void)
		{
			return m_model;
		}

		nodes::Node* Mesh::node(uint32_t index) const
		{
			return m_nodes[index];
		}
		elements::Element* Mesh::element(uint32_t index) const
		{
			return m_elements[index];
		}

		const std::vector<nodes::Node*>& Mesh::nodes(void) const
		{
			return m_nodes;
		}
		const std::vector<elements::Element*>& Mesh::elements(void) const
		{
			return m_elements;
		}

		//create
		void Mesh::create_node(const double* position_ref)
		{
			//data
			const uint32_t nn = m_nodes.size();
			nodes::Node* node = new nodes::Node(position_ref);
			//append
			node->m_index = nn;
			m_nodes.push_back(node);
		}
		void Mesh::create_node(double x1, double x2, double x3)
		{
			//data
			const uint32_t nn = m_nodes.size();
			nodes::Node* node = new nodes::Node(x1, x2, x3);
			//append
			node->m_index = nn;
			m_nodes.push_back(node);
		}

		void Mesh::create_element(elements::Type type, std::vector<uint32_t> nodes)
		{
			//data
			elements::Element* element;
			const uint32_t ne = m_elements.size();
			std::function<void(elements::Element*&)> fabric[] = {
				[](elements::Element*& element){ element = new elements::Truss2D; },
				[](elements::Element*& element){ element = new elements::Truss3D; }
			};
			//create
			fabric[uint32_t(type)](element);
			//append
			element->m_index = ne;
			element->m_nodes = nodes;
			m_elements.push_back(element);
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
		void Mesh::compute(void)
		{
			for(nodes::Node* node : m_nodes) node->compute();
			for(elements::Element* element : m_elements) element->compute();
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
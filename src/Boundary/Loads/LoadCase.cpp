//FEA
#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/Element.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		LoadCase::LoadCase(void)
		{
			return;
		}
	
		//destructor
		LoadCase::~LoadCase(void)
		{
			for(const loads::Node* load : m_loads_nodes) delete load;
			for(const loads::Element* load : m_loads_elements) delete load;
		}

		//serialization
		void LoadCase::save(FILE* file) const
		{
			//data
			const uint32_t nn = m_loads_nodes.size();
			const uint32_t ne = m_loads_elements.size();
			//header
			fprintf(file, "\n");
			fprintf(file, "### Load Case %d:\n", m_index);
			//nodes
			fprintf(file, "\n");
			fprintf(file, "#### Loads on nodes: %d\n", nn);
			for(const loads::Node* load : m_loads_nodes) load->save(file), fprintf(file, "\n");
			//elements
			fprintf(file, "\n");
			fprintf(file, "#### Loads on elements: %d\n", ne);
			for(const loads::Element* load : m_loads_elements) load->save(file), fprintf(file, "\n");
		}

		//data
		Boundary* LoadCase::boundary(void)
		{
			return m_boundary;
		}

		loads::Node* LoadCase::load_node(uint32_t index) const
		{
			return m_loads_nodes[index];
		}
		loads::Element* LoadCase::load_element(uint32_t index) const
		{
			return m_loads_elements[index];
		}

		const std::vector<loads::Node*>& LoadCase::loads_nodes(void) const
		{
			return m_loads_nodes;
		}
		const std::vector<loads::Element*>& LoadCase::loads_elements(void) const
		{
			return m_loads_elements;
		}

		//create
		void LoadCase::create_load_node(uint32_t node, mesh::nodes::DOF dof, double value)
		{
			m_loads_nodes.push_back(new loads::Node{node, dof, value});
		}

		//analysis
		void LoadCase::check(void)
		{
			for(loads::Node* load : m_loads_nodes) load->check();
			for(loads::Element* load : m_loads_elements) load->check();
		}
		void LoadCase::setup(void)
		{
			for(loads::Node* load : m_loads_nodes) load->setup();
			for(loads::Element* load : m_loads_elements) load->setup();
		}

		//static
		Boundary* LoadCase::m_boundary = nullptr;
	}
}
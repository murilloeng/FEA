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
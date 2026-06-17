//std
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stdexcept>

//FEA
#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Loads/Nodes/Node.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Element.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			LoadCase::LoadCase(void) : m_label("LoadCase")
			{
				return;
			}

			//destructor
			LoadCase::~LoadCase(void)
			{
				//delete loads nodes
				for(loads::Node*& load_node : m_loads_nodes)
				{
					delete load_node;
				}
				//delete loads elements
				for(loads::Element* load_element : m_loads_elements)
				{
					delete load_element;
				}
			}

			//serialization
			void LoadCase::load(FILE* file)
			{
				//load label
				if(fscanf(file, "%s", m_label) != 1)
				{
					throw std::runtime_error("Load case loading error!");
				}
				//load sizes
				uint32_t nn, ne;
				if(fscanf(file, "%d %d", &nn, &ne) != 2)
				{
					throw std::runtime_error("Load case loading error!");
				}
				//load nodes
				m_loads_nodes.resize(nn);
				for(loads::Node*& load_node : m_loads_nodes)
				{
					load_node = new loads::Node(this);
					load_node->load(file);
				}
				//load elements
				uint32_t type;
				m_loads_elements.resize(ne);
				for(loads::Element*& load_element : m_loads_elements)
				{
					if(fscanf(file, "%d", &type) != 1)
					{
						throw std::runtime_error("Load case loading error!");
					}
					loads::Element::create(load_element, Type(type), this);
					load_element->load(file);
				}
			}
			void LoadCase::save(FILE* file) const
			{
				//save label
				fprintf(file, "%s ", m_label);
				//save sizes
				fprintf(file, "%04d ", (uint32_t) m_loads_nodes.size());
				fprintf(file, "%04d\n", (uint32_t) m_loads_elements.size());
				//save nodes
				for(const loads::Node* load_node : m_loads_nodes)
				{
					load_node->save(file);
					fprintf(file, "\n");
				}
				//save elements
				for(const loads::Element* load_element : m_loads_elements)
				{
					fprintf(file, "%02d ", (uint32_t) load_element->type());
					load_element->save(file);
					fprintf(file, "\n");
				}
			}

			//data
			const char* LoadCase::label(void) const
			{
				return m_label;
			}
			const char* LoadCase::label(const char* label)
			{
				return strcpy(m_label, label);
			}

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

			//lists
			const std::vector<loads::Node*>& LoadCase::loads_nodes(void) const
			{
				return m_loads_nodes;
			}
			const std::vector<loads::Element*>& LoadCase::loads_elements(void) const
			{
				return m_loads_elements;
			}

			//create
			void LoadCase::create_load_element(uint32_t element, Type type, double value, uint32_t time)
			{
				//load
				loads::Element* load = nullptr;
				loads::Element::create(load, type, this);
				//setup
				load->m_time = time;
				load->m_value = value;
				load->m_element = element;
				m_loads_elements.push_back(load);
			}
			void LoadCase::create_load_node(uint32_t node, mesh::nodes::dof dof, double value, uint32_t time)
			{
				//load
				loads::Node* load = new loads::Node(this);
				//setup
				load->m_dof = dof;
				load->m_node = node;
				load->m_time = time;
				load->m_value = value;
				m_loads_nodes.push_back(load);
			}

			//remove
			void LoadCase::remove_load_node(uint32_t index)
			{
				delete m_loads_nodes[index];
				m_loads_nodes.erase(m_loads_nodes.begin() + index);
			}
			void LoadCase::remove_load_element(uint32_t index)
			{
				delete m_loads_elements[index];
				m_loads_elements.erase(m_loads_elements.begin() + index);
			}

			void LoadCase::remove_load_node(const Node* load)
			{
				delete load;
				std::vector<Node*>& vector = m_loads_nodes;
				vector.erase(std::remove(vector.begin(), vector.end(), load), vector.end());
			}
			void LoadCase::remove_load_element(const Element* load)
			{
				delete load;
				std::vector<Element*>& vector = m_loads_elements;
				vector.erase(std::remove(vector.begin(), vector.end(), load), vector.end());
			}

			//index
			uint32_t LoadCase::index(void) const
			{
				return m_index;
			}

			//analysis
			bool LoadCase::check(void) const
			{
				//check node loads
				for(const loads::Node* load_node : m_loads_nodes)
				{
					if(!load_node->check())
					{
						return false;
					}
				}
				//check element loads
				for(const loads::Element* load_element : m_loads_elements)
				{
					if(!load_element->check())
					{
						return false;
					}
				}
				//return
				return true;
			}
			void LoadCase::apply_dof(void) const
			{
				for(loads::Node* load_node : m_loads_nodes) load_node->apply_dof();
			}
			void LoadCase::setup(void)
			{
				for(loads::Node* load_node : m_loads_nodes)
				{
					load_node->prepare();
				}
				for(loads::Element* load_element : m_loads_elements)
				{
					load_element->prepare();
				}
			}

			//static data
			Boundary* LoadCase::m_boundary = nullptr;
		}
	}
}
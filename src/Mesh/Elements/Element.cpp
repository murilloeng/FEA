//std
#include <stdexcept>

//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Element::Element(void) : m_index{0}
			{
				return;
			}

			//destructor
			Element::~Element(void)
			{
				return;
			}

			//serialization
			void Element::load(FILE* file)
			{
				//size
				uint32_t nn;
				if(fscanf(file, "%d", &nn) != 1)
				{
					throw std::runtime_error("Error: Element's loading failed!");
				}
				//load
				m_nodes.resize(nn);
				for(uint32_t& node : m_nodes)
				{
					if(fscanf(file, "%d", &node) != 1)
					{
						throw std::runtime_error("Error: Element's loading failed!");
					}
				}
			}
			void Element::save(FILE* file) const
			{
				if(fprintf(file, "%zd ", m_nodes.size()) < 0)
				{
					throw std::runtime_error("Error: Element's saving failed!");
				}
				for(uint32_t node : m_nodes)
				{
					if(fprintf(file, "%d ", node) < 0)
					{
						throw std::runtime_error("Error: Element's saving failed!");
					}
				}
				fprintf(file, "\n");
			}

			//data
			uint32_t Element::index(void) const
			{
				return m_index;
			}

			nodes::Node* Element::node(uint32_t index) const
			{
				return m_mesh->node(m_nodes[index]);
			}
			nodes::Node* Element::node(uint32_t index, uint32_t node)
			{
				return m_mesh->node(m_nodes[index] = node);
			}
			const std::vector<uint32_t>& Element::nodes(void) const
			{
				return m_nodes;
			}

			//analysis
			void Element::check(void)
			{
				return;
			}
			void Element::setup(void)
			{
				return;
			}
			void Element::record(void)
			{
				return;
			}
			void Element::update(void)
			{
				return;
			}
			void Element::restore(void)
			{
				return;
			}
		}
	}
}
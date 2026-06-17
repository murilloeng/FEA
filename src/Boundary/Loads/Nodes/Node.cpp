//std
#include <cmath>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Loads/Nodes/Node.hpp"

//static data
static const uint32_t dof_r = 
	uint32_t(fea::mesh::nodes::dof::rotation_1) |
	uint32_t(fea::mesh::nodes::dof::rotation_2) |
	uint32_t(fea::mesh::nodes::dof::rotation_3);
static const uint32_t dof_t = 
	uint32_t(fea::mesh::nodes::dof::translation_1) |
	uint32_t(fea::mesh::nodes::dof::translation_2) |
	uint32_t(fea::mesh::nodes::dof::translation_3);

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			Node::Node(LoadCase* load_case) : Load(load_case)
			{
				return;
			}

			//destructor
			Node::~Node(void)
			{
				return;
			}

			//serialization
			void Node::load(FILE* file)
			{
				uint32_t dof;
				Load::load(file);
				if(fscanf(file, "%d %d", &m_node, &dof) != 2)
				{
					throw std::runtime_error("Nodal load loading error!");
				}
				m_dof = mesh::nodes::dof(dof);
			}
			void Node::save(FILE* file) const
			{
				Load::save(file);
				fprintf(file, "%04d %02d ", m_node, (uint32_t) m_dof);
			}

			//data
			mesh::nodes::dof Node::dof(void) const
			{
				return m_dof;
			}
			mesh::nodes::dof Node::dof(mesh::nodes::dof dof)
			{
				return m_dof = dof;
			}

			mesh::nodes::Node* Node::node(void) const
			{
				return m_load_case->boundary()->model()->mesh()->node(m_node);
			}
			mesh::nodes::Node* Node::node(uint32_t node)
			{
				return m_load_case->boundary()->model()->mesh()->node(m_node = node);
			}

			//index
			uint32_t Node::index(void) const
			{
				return m_index;
			}
			uint32_t Node::index_node(void) const
			{
				return m_node;
			}

			//analysis
			bool Node::check(void) const
			{
				if(m_node >= m_load_case->boundary()->model()->mesh()->nodes().size())
				{
					printf("Node load %02d of Load case %02d has out of range node\n", m_index, m_load_case->index());
					return false;
				}
				return true;
			}
			void Node::prepare(void)
			{
				const mesh::nodes::Node* node = m_load_case->boundary()->model()->mesh()->node(m_node);
				const uint8_t p = math::bit_index(node->m_dof_set, (uint32_t) m_dof);
				m_dof_index = node->m_dof_index[p];
			}
			void Node::apply_dof(void) const
			{
				m_load_case->boundary()->model()->mesh()->node(m_node)->apply_dof(m_dof);
			}
		}
	}
}
//std
#include <cmath>
#include <cfloat>
#include <cstring>
#include <algorithm>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Miscellaneous/util.hpp"
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Linear/Mat3.hpp"
#include "Math/inc/Linear/Quat.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"

#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Mesh/Joints/Joint.hpp"

#include "FEA/inc/Mesh/Elements/Element.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace mesh
	{
		//constructors
		Mesh::Mesh(void)
		{
			//mesh
			nodes::Node::m_mesh = this;
			joints::Joint::m_mesh = this;
			elements::Element::m_mesh = this;
		}

		//destructor
		Mesh::~Mesh(void)
		{
			//delete nodes
			for(const nodes::Node* node : m_nodes)
			{
				delete node;
			}
			//delete joints
			for(const joints::Joint* joint : m_joints)
			{
				delete joint;
			}
			//delete elements
			for(const elements::Element* element : m_elements)
			{
				delete element;
			}
		}

		//serialization
		void Mesh::load(FILE* file)
		{
			//header
			math::skip_lines(file, 2);
			//objects
			load_nodes(file);
			load_joints(file);
			load_elements(file);
		}
		void Mesh::load_nodes(FILE* file)
		{
			uint32_t size;
			if(fscanf(file, "## Nodes: %d", &size) != 1)
			{
				throw std::runtime_error("Mesh nodes count loading error!");
			}
			for(uint32_t i = 0; i < size; i++)
			{
				create_node(0, 0, 0);
				m_nodes[i]->load(file);
			}
			math::skip_lines(file, 2);
		}
		void Mesh::load_joints(FILE* file)
		{
			uint32_t size, type;
			if(fscanf(file, "## Joints: %d", &size) != 1)
			{
				throw std::runtime_error("Mesh joints count loading error!");
			}
			for(uint32_t i = 0; i < size; i++)
			{
				if(fscanf(file, "%d", &type) != 1)
				{
					throw std::runtime_error("Mesh joint's type loading error!");
				}
				create_joint(mesh::joints::Type(type));
				m_joints[i]->load(file);
			}
			math::skip_lines(file, 2);
		}
		void Mesh::load_elements(FILE* file)
		{
			uint32_t size, type;
			if(fscanf(file, "## Elements: %d", &size) != 1)
			{
				throw std::runtime_error("Mesh elements count loading error!");
			}
			for(uint32_t i = 0; i < size; i++)
			{
				if(fscanf(file, "%d", &type) != 1)
				{
					throw std::runtime_error("Mesh element's type loading error!");
				}
				create_element(mesh::elements::Type(type));
				m_elements[i]->load(file);
			}
			math::skip_lines(file, 2);
		}

		void Mesh::save(FILE* file) const
		{
			//header
			fprintf(file, "# MESH\n\n");
			//objects
			save_nodes(file);
			save_joints(file);
			save_elements(file);
		}
		void Mesh::save_nodes(FILE* file) const
		{
			fprintf(file, "## Nodes: %zd\n", m_nodes.size());
			for(const nodes::Node* node : m_nodes)
			{
				node->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}
		void Mesh::save_joints(FILE* file) const
		{
			fprintf(file, "## Joints: %zd\n", m_joints.size());
			for(const joints::Joint* joint : m_joints)
			{
				fprintf(file, "%04d ", (uint32_t) joint->type());
				joint->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}
		void Mesh::save_elements(FILE* file) const
		{
			fprintf(file, "## Elements: %zd\n", m_elements.size());
			for(const elements::Element* element : m_elements)
			{
				fprintf(file, "%04d ", (uint32_t) element->type());
				element->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}

		void Mesh::load_results(void)
		{
			for(nodes::Node* node : m_nodes)
			{
				node->load_results();
			}
			for(joints::Joint* joint : m_joints)
			{
				joint->load_state();
				joint->load_energy();
			}
			for(elements::Element* element : m_elements)
			{
				element->load_state();
				element->load_energy();
			}
		}
		void Mesh::save_results(void) const
		{
			for(const nodes::Node* node : m_nodes)
			{
				node->save_results();
			}
			for(const joints::Joint* joint : m_joints)
			{
				joint->save_state();
				joint->save_energy();
			}
			for(const elements::Element* element : m_elements)
			{
				element->save_state();
				element->save_energy();
			}
		}

		//data
		Model* Mesh::model(void) const
		{
			return m_model;
		}
		nodes::Node* Mesh::node(uint32_t index) const
		{
			return index < m_nodes.size() ? m_nodes[index] : nullptr;
		}
		joints::Joint* Mesh::joint(uint32_t index) const
		{
			return index < m_joints.size() ? m_joints[index] : nullptr;
		}
		elements::Element* Mesh::element(uint32_t index) const
		{
			return index < m_elements.size() ? m_elements[index] : nullptr;
		}

		uint32_t Mesh::step(void) const
		{
			return m_model->analysis()->solver()->step();
		}

		//sizes
		const std::vector<nodes::Node*>& Mesh::nodes(void) const
		{
			return m_nodes;
		}
		const std::vector<joints::Joint*>& Mesh::joints(void) const
		{
			return m_joints;
		}
		const std::vector<elements::Element*>& Mesh::elements(void) const
		{
			return m_elements;
		}

		//create
		nodes::Node* Mesh::create_node(const double* p)
		{
			//data
			nodes::Node* node = new nodes::Node;
			//node
			node->coordinates(p);
			node->m_index = m_nodes.size();
			//list
			m_nodes.push_back(node);
			//return
			return node;
		}
		nodes::Node* Mesh::create_node(double x, double y, double z)
		{
			//data
			nodes::Node* node = new nodes::Node;
			//node
			node->coordinates(x, y, z);
			node->m_index = m_nodes.size();
			//list
			m_nodes.push_back(node);
			//return
			return node;
		}
		joints::Joint* Mesh::create_joint(joints::Type type, std::vector<uint32_t> nodes)
		{
			//data
			joints::Joint* joint = nullptr;
			joints::Joint::create(joint, type);
			//joint
			joint->m_nodes = nodes;
			joint->m_index = m_joints.size();
			//list
			m_joints.push_back(joint);
			//return
			return joint;
		}
		elements::Element* Mesh::create_element(const elements::Element* base)
		{
			//data
			elements::Element* element = nullptr;
			elements::Element::create(element, base);
			//element
			element->m_index = m_elements.size();
			//list
			m_elements.push_back(element);
			//return
			return element;
		}
		elements::Element* Mesh::create_element(elements::Type type, std::vector<uint32_t> nodes)
		{
			//data
			elements::Element* element = nullptr;
			elements::Element::create(element, type);
			//element
			element->m_nodes = nodes;
			element->m_index = m_elements.size();
			//list
			m_elements.push_back(element);
			//return
			return element;
		}

		//remove
		void Mesh::remove_node(uint32_t index)
		{
			//set joints
			for(int32_t i = m_joints.size() - 1; i >= 0; i--)
			{
				std::vector<uint32_t>& nodes = m_joints[i]->m_nodes;
				if(std::find(nodes.begin(), nodes.end(), index) != nodes.end())
				{
					remove_joint(i);
				}
				else
				{
					for(uint32_t& node : nodes)
					{
						if(node > index) node--;
					}
				}
			}
			//set elements
			for(int32_t i = m_elements.size() - 1; i >= 0; i--)
			{
				std::vector<uint32_t>& nodes = m_elements[i]->m_nodes;
				if(std::find(nodes.begin(), nodes.end(), index) != nodes.end())
				{
					remove_element(i);
				}
				else
				{
					for(uint32_t& node : nodes)
					{
						if(node > index) node--;
					}
				}
			}
			//indexes
			m_model->boundary()->remove_node(index);
			for(uint32_t i = 0; i < m_nodes.size(); i++)
			{
				m_nodes[i]->m_index--;
			}
			//remove node
			delete m_nodes[index];
			m_nodes.erase(m_nodes.begin() + index);
		}
		void Mesh::remove_joint(uint32_t index)
		{
			//indexes
			for(uint32_t i = 0; i < m_joints.size(); i++)
			{
				m_joints[i]->m_index--;
			}
			//remove
			delete m_joints[index];
			m_joints.erase(m_joints.begin() + index);
		}
		void Mesh::remove_element(uint32_t index)
		{
			//indexes
			m_model->boundary()->remove_element(index);
			for(uint32_t i = 0; i < m_elements.size(); i++)
			{
				m_elements[i]->m_index--;
			}
			//remove
			delete m_elements[index];
			m_elements.erase(m_elements.begin() + index);
		}

		void Mesh::remove_nodes(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_node(list[i]);
			}
		}
		void Mesh::remove_joints(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_joint(list[i]);
			}
		}
		void Mesh::remove_elements(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_element(list[i]);
			}
		}

		//analysis
		void Mesh::check(void)
		{
			for(nodes::Node* node : m_nodes) node->check();
			for(joints::Joint* joint : m_joints) joint->check();
			for(elements::Element* element : m_elements) element->check();
		}

		void Mesh::setup(void)
		{
			for(nodes::Node* node : m_nodes) node->setup();
			for(joints::Joint* joint : m_joints) joint->setup();
			for(elements::Element* element : m_elements) element->setup();
		}
		void Mesh::setup_dof(uint32_t& dof_counter) const
		{
			for(nodes::Node* node : m_nodes) node->setup_dof(dof_counter);
		}

		void Mesh::record(void)
		{
			//nodes
			for(nodes::Node* node : m_nodes)
			{
				node->record();
			}
			//joints
			for(joints::Joint* joint : m_joints)
			{
				joint->record();
			}
			//elements
			for(elements::Element* element : m_elements)
			{
				element->record();
			}
		}
		void Mesh::update(void)
		{
			//nodes
			for(nodes::Node* node : m_nodes)
			{
				node->update();
			}
			//joints
			for(joints::Joint* joint : m_joints)
			{
				joint->update();
			}
			//elements
			for(elements::Element* element : m_elements)
			{
				element->update();
			}
		}
		void Mesh::restore(void)
		{
			//nodes
			for(nodes::Node* node : m_nodes)
			{
				node->restore();
			}
			//joints
			for(joints::Joint* joint : m_joints)
			{
				joint->restore();
			}
			//elements
			for(elements::Element* element : m_elements)
			{
				element->restore();
			}
		}
		void Mesh::apply_dof(void)
		{
			for(const joints::Joint* joint : m_joints) joint->apply_dof();
			for(const elements::Element* element : m_elements) element->apply_dof();
		}

		//static data
		Model* Mesh::m_model = nullptr;
	}
}
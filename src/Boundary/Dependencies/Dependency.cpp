//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Initials/Initial.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"
#include "FEA/inc/Boundary/Dependencies/Dependency.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Dependency::Dependency(void)
		{
			return;
		}

		//destructor
		Dependency::~Dependency(void)
		{
			return;
		}

		//serialization
		void Dependency::save(FILE* file) const
		{
			fprintf(file, "Index: %d Nodes: %d %d DOF: %d %d ", m_index, m_nodes[0], m_nodes[1], uint32_t(m_dof[0]), uint32_t(m_dof[1]));
		}

		//data
		mesh::nodes::Node* Dependency::node(uint32_t index) const
		{
			return m_boundary->model()->mesh()->node(m_nodes[index]);
		}
		mesh::nodes::Node* Dependency::node(uint32_t index, uint32_t node)
		{
			return m_boundary->model()->mesh()->node(m_nodes[index] = node);
		}

		mesh::nodes::DOF Dependency::dof(uint32_t index) const
		{
			return m_dof[index];
		}
		mesh::nodes::DOF Dependency::dof(uint32_t index, mesh::nodes::DOF dof)
		{
			return m_dof[index] = dof;
		}

		//index
		uint32_t Dependency::index(void) const
		{
			return m_index;
		}

		uint32_t Dependency::index_node(uint32_t index) const
		{
			return m_nodes[index];
		}
		uint32_t Dependency::index_node(uint32_t index, uint32_t node)
		{
			return m_nodes[index] = node;
		}

		//analysis
		void Dependency::check(void)
		{
			check_inner();
			check_outer();
			check_nodes();
			check_initials();
			check_supports();
		}
		void Dependency::setup(void)
		{
			return;
		}

		//check
		void Dependency::check_inner(void) const
		{
			if(m_nodes[0] == m_nodes[1] && m_dof[0] == m_dof[1])
			{
				throw std::runtime_error("Error: Dependency is inner inconsistent!");
			}
		}
		void Dependency::check_outer(void) const
		{
			for(const Dependency* dependency : m_boundary->dependencies())
			{
				if(dependency != this && *dependency == *this)
				{
					throw std::runtime_error("Error: Dependency is outer inconsistent!");
				}
			}
		}
		void Dependency::check_nodes(void) const
		{
			//data
			const uint32_t nn = m_boundary->model()->mesh()->nodes().size();
			//check
			if(m_nodes[0] >= nn || m_nodes[1] >= nn)
			{
				throw std::runtime_error("Error: Dependency has out of range nodes!");
			}
		}
		void Dependency::check_initials(void) const
		{
			bool test_1 = false, test_2 = false;
			for(const Initial* initial : m_boundary->initials())
			{
				test_1 = test_1 || (m_nodes[0] == initial->index_node() && m_dof[0] == initial->dof());
				test_2 = test_2 || (m_nodes[1] == initial->index_node() && m_dof[1] == initial->dof());
			}
			if(test_1 && test_2)
			{
				throw std::runtime_error("Error: Dependency is inconsistent with initials!");
			}
		}
		void Dependency::check_supports(void) const
		{
			bool test_1 = false, test_2 = false;
			for(const Support* support : m_boundary->supports())
			{
				test_1 = test_1 || (m_nodes[0] == support->index_node() && m_dof[0] == support->dof());
				test_2 = test_2 || (m_nodes[1] == support->index_node() && m_dof[1] == support->dof());
			}
			if(test_1 && test_2)
			{
				throw std::runtime_error("Error: Dependency is inconsistent with supports!");
			}
		}

		//operators
		bool Dependency::operator==(const Dependency& dependency) const
		{
			//data
			const bool test_1 =
				m_nodes[0] == dependency.m_nodes[0] && m_dof[0] == dependency.m_dof[0] &&
				m_nodes[1] == dependency.m_nodes[1] && m_dof[1] == dependency.m_dof[1];
			const bool test_2 =
				m_nodes[0] == dependency.m_nodes[1] && m_dof[0] == dependency.m_dof[1] &&
				m_nodes[1] == dependency.m_nodes[0] && m_dof[1] == dependency.m_dof[0];
				//return
			return test_1 || test_2;
		}

		//static
		Boundary* Dependency::m_boundary = nullptr;
	}
}
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
		uint32_t Dependency::dof_index(bool flag) const
		{
			return 
				m_nodes[0] < m_nodes[1] || (m_nodes[0] == m_nodes[1] && m_dof[0] < m_dof[1]) ? 
				m_boundary->model()->m_mesh->node(m_nodes[flag])->dof_index(m_dof[flag]) : 
				m_boundary->model()->m_mesh->node(m_nodes[!flag])->dof_index(m_dof[!flag]);
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
			const uint32_t nn = m_boundary->model()->m_mesh->nodes().size();
			//check
			if(m_nodes[0] >= nn || m_nodes[1] >= nn)
			{
				throw std::runtime_error("Error: Dependency has out of range nodes!");
			}
		}
		void Dependency::check_initials(void) const
		{
			bool test = true;
			for(uint32_t i = 0; i < 2; i++)
			{
				for(const Initial* initial : m_boundary->initials())
				{
					test = test && (m_nodes[i] != initial->index_node() || m_dof[i] != initial->dof());
				}
			}
			if(!test)
			{
				throw std::runtime_error("Error: Dependency is inconsistent with supports!");
			}
		}
		void Dependency::check_supports(void) const
		{
			bool test = true;
			for(uint32_t i = 0; i < 2; i++)
			{
				for(const Support* support : m_boundary->supports())
				{
					test = test && (m_nodes[i] != support->index_node() || m_dof[i] != support->dof());
				}
			}
			if(!test)
			{
				throw std::runtime_error("Error: Dependency is inconsistent with supports!");
			}
		}

		//operators
		bool Dependency::operator==(const Dependency& dependency) const
		{
			const bool test[] = {
				m_nodes[0] == dependency.m_nodes[0] && m_dof[0] == dependency.m_dof[0] &&
				m_nodes[1] == dependency.m_nodes[1] && m_dof[1] == dependency.m_dof[1],
				m_nodes[0] == dependency.m_nodes[1] && m_dof[0] == dependency.m_dof[1] &&
				m_nodes[1] == dependency.m_nodes[0] && m_dof[1] == dependency.m_dof[0]
			};
			return test[0] || test[1];
		}

		//static
		Boundary* Dependency::m_boundary = nullptr;
	}
}
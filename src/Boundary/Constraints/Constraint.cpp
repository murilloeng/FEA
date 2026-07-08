//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Constraint::Constraint(void) : m_dof_index{0}
		{
			return;
		}
	
		//destructor
		Constraint::~Constraint(void)
		{
			return;
		}

		//analysis
		void Constraint::check(void)
		{
			for(uint32_t node : m_nodes)
			{
				if(node >= m_boundary->model()->m_mesh->nodes().size())
				{
					throw std::runtime_error("Error: Constraint's node is out of range!");
				}
			}
			if(m_nodes.size() != m_dof.size())
			{
				throw std::runtime_error("Error: Constraint's nodes and dofs lists are incompatible!");
			}
		}
		void Constraint::setup(void)
		{
			m_dof_indexes.clear();
			for(uint32_t i = 0; i < m_nodes.size(); i++)
			{
				m_dof_indexes.push_back(m_boundary->model()->m_mesh->node(m_nodes[i])->dof_index(m_dof[i]));
			}
		
		}
		void Constraint::dof_setup(uint32_t& dof_counter)
		{
			m_dof_index = dof_counter++;
		}

		//static
		Boundary* Constraint::m_boundary = nullptr;
	}
}
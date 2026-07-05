//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Boundary/Initials/Initial.hpp"

#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"

#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

#include "FEA/inc/Boundary/Dependencies/Dependency.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Boundary::Boundary(void)
		{
			Initial::m_boundary = this;
			Support::m_boundary = this;
			LoadCase::m_boundary = this;
			Constraint::m_boundary = this;
			Dependency::m_boundary = this;
			LoadCombination::m_boundary = this;
		}
		
		//destructor
		Boundary::~Boundary(void)
		{
			for(const Initial* initial : m_initials) delete initial;
			for(const Support* support : m_supports) delete support;
			for(const LoadCase* load_case : m_load_cases) delete load_case;
			for(const Constraint* constraint : m_constraints) delete constraint;
			for(const Dependency* dependency : m_dependencies) delete dependency;
			for(const LoadCombination* load_combination : m_load_combinations) delete load_combination;
		}

		//analysis
		void Boundary::apply_dof(void)
		{
			//initials
			for(const Initial* initial : m_initials)
			{
				m_model->m_mesh->m_nodes[initial->m_node]->m_dof |= 1 << uint32_t(initial->m_dof);
			}
			//supports
			for(const Support* support : m_supports)
			{
				m_model->m_mesh->m_nodes[support->m_node]->m_dof |= 1 << uint32_t(support->m_dof);
			}
			//load cases
			for(const LoadCase* load_case : m_load_cases)
			{
				for(const loads::Node* load : load_case->m_loads_nodes)
				{
					m_model->m_mesh->m_nodes[load->m_node]->m_dof |= 1 << uint32_t(load->m_dof);
				}
			}
			//constraints
			for(const Constraint* constraint : m_constraints)
			{
				for(uint32_t i = 0; i < constraint->m_nodes.size(); i++)
				{
					m_model->m_mesh->m_nodes[constraint->m_nodes[i]]->m_dof |= 1 << uint32_t(constraint->m_dof[i]);
				}
			}
			//dependencies
			for(const Dependency* dependency : m_dependencies)
			{
				m_model->m_mesh->m_nodes[dependency->m_nodes[0]]->m_dof |= 1 << uint32_t(dependency->m_dof[0]);
				m_model->m_mesh->m_nodes[dependency->m_nodes[1]]->m_dof |= 1 << uint32_t(dependency->m_dof[1]);
			}
		}

		//static
		Model* Boundary::m_model = nullptr;
	}
}
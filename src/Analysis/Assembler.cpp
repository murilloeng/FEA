//std
#include <cstring>

//SuiteSparse
#include <suitesparse/umfpack.h>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Assembler::Assembler(void) : 
			m_fe{nullptr}, m_Ae{nullptr},
			m_rows_map{nullptr}, m_cols_map{nullptr}, 
			m_rows_triplet{nullptr}, m_cols_triplet{nullptr},
			m_dof_know{0}, m_dof_local{0}, m_dof_total{0}, m_dof_unknow{0}, m_dof_triplet{0}, m_dof_dependent{0}
		{
			return;
		}
		
		//destructor
		Assembler::~Assembler(void)
		{
			delete[] m_fe;
			delete[] m_Ae;
			delete[] m_rows_map;
			delete[] m_cols_map;
			delete[] m_rows_triplet;
			delete[] m_cols_triplet;
		}

		//dof
		void Assembler::map_dof(void)
		{
			// //map dof
			// count_dof();
			// m_rows_map = new int32_t[m_dof_triplet];
			// m_cols_map = new int32_t[m_dof_unknow + 1];
			// m_rows_triplet = new int32_t[m_dof_triplet];
			// m_cols_triplet = new int32_t[m_dof_triplet];
			// memset(m_cols_map, 0, (m_dof_unknow + 1) * sizeof(int32_t));
			// for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			// {
			// 	add_dof(element->m_dof_index);
			// }
			// for(const boundary::Support* support : m_analysis->m_model->m_boundary->m_supports)
			// {
			// 	add_dof(support->m_dof_index, support->m_dof_index);
			// }
			// for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			// {
			// 	add_dof(constraint->dof_list());
			// }
			//sparse format
			const uint32_t nu = m_dof_unknow;
			const uint32_t nz = m_cols_map[m_dof_unknow];
			umfpack_di_triplet_to_col(nu, nu, nz, m_rows_triplet, m_cols_triplet, nullptr, m_cols_map, m_rows_map, nullptr, nullptr);
		}

		void Assembler::count_dof(void)
		{
			// for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			// {
			// 	count_dof(joint->m_dof_index);
			// }
			// for(const boundary::Support* support : m_analysis->m_model->m_boundary->m_supports)
			// {
			// 	count_dof(support->m_dof_index);
			// }
			// for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			// {
			// 	count_dof(element->m_dof_index);
			// }
			// for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			// {
			// 	count_dof(constraint->dof_list());
			// }
		}
		void Assembler::count_dof(uint32_t)
		{
			return;
		}
		// void count_dof(const ulist&);

		// void add_dof(const ulist&);
		void Assembler::add_dof(uint32_t, uint32_t)
		{
			return;
		}

		void Assembler::sort_dof(void)
		{
			return;
		}
		void Assembler::save_dof(void)
		{
			return;
		}
		void Assembler::apply_dof(void)
		{
			//clear
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->m_dof = 0;
			}
			//apply
			m_analysis->apply_dof();
			m_analysis->m_model->m_mesh->apply_dof();
			m_analysis->m_model->m_boundary->apply_dof();
		}
		void Assembler::setup_dof(void)
		{
			return;
		}
			// void sort_dof_lists(ulist&, ulist&) const;
			// void sort_dof_nodes(const ulist&, const ulist&) const;
		void Assembler::sort_dof_constraints(void) const
		{
			return;
		}

		//static
		Analysis* Assembler::m_analysis = nullptr;
	}
}
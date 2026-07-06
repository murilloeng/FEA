//std
#include <cstring>
#include <algorithm>

//SuiteSparse
#include <suitesparse/umfpack.h>

//Math
#include "Math/inc/Linear/Sparse.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"
#include "FEA/inc/Boundary/Dependencies/Dependency.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

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
		void Assembler::dof_map(void)
		{
			//mapping
			dof_triplet_count();
			int32_t* rm = m_rows_map = new int32_t[m_dof_triplet];
			int32_t* cm = m_cols_map = new int32_t[m_dof_unknow + 1];
			int32_t* rt = m_rows_triplet = new int32_t[m_dof_triplet];
			int32_t* ct = m_cols_triplet = new int32_t[m_dof_triplet];
			//sparse format
			dof_triplet_apply();
			const uint32_t nu = m_dof_unknow;
			const uint32_t nz = m_dof_triplet;
			umfpack_di_triplet_to_col(nu, nu, nz, rt, ct, nullptr, cm, rm, nullptr, nullptr);
		}
		void Assembler::dof_sort(void)
		{
			//data
			dof_apply();
			dof_setup();
			uint32_t cd = 0, ck = 0;
			std::vector<uint32_t> dd, dk;
			const std::vector<boundary::Dependency*> dependencies = m_analysis->m_model->m_boundary->m_dependencies;
			//lists
			for(const boundary::Support* support : m_analysis->m_model->m_boundary->m_supports)
			{
				dk.push_back(support->node()->dof_index(support->m_dof));
			}
			for(const boundary::Dependency* dependency : m_analysis->m_model->m_boundary->m_dependencies)
			{
				dd.push_back(dependency->dof_index(true));
			}
			//nodes
			m_dof_know = (uint32_t) dk.size();
			m_dof_dependent = (uint32_t) dd.size();
			m_dof_unknow = m_dof_total - m_dof_know - m_dof_dependent;
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				for(uint32_t& dof_index : node->m_dof_indexes)
				{
					std::vector<uint32_t>::iterator pd = std::find(dd.begin(), dd.end(), dof_index);
					std::vector<uint32_t>::iterator pk = std::find(dk.begin(), dk.end(), dof_index);
					if(pd != dd.end() ? ++cd : false)
					{
						dof_index = dependencies[std::distance(dd.begin(), pd)]->dof_index(false);
					}
					else if(pk != dk.end() ? ++ck : false)
					{
						dof_index = m_dof_unknow + (uint32_t) std::distance(dk.begin(), pk);
					}
					else
					{
						dof_index -= cd + ck;
					}
				}
			}
			//constraintes
			for(boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				constraint->m_dof_index -= m_dof_know + m_dof_dependent;
			}
		}
		void Assembler::dof_apply(void)
		{
			//clear
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->m_dof_set = 0;
			}
			//apply
			m_analysis->dof_apply();
			m_analysis->m_model->m_mesh->dof_apply();
			m_analysis->m_model->m_boundary->dof_apply();
		}
		void Assembler::dof_setup(void)
		{
			m_dof_total = 0;
			m_analysis->m_model->m_mesh->dof_setup(m_dof_total);
			m_analysis->m_model->m_boundary->dof_setup(m_dof_total);
		}
		void Assembler::dof_local(void)
		{
			//count
			m_dof_local = 0;
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				m_dof_local = std::max(std::size_t(m_dof_local), element->m_dof_indexes.size());
			}
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				m_dof_local = std::max(std::size_t(m_dof_local), constraint->m_dof_indexes.size());
			}
			//allocate
			delete[] m_fe;
			delete[] m_Ae;
			m_fe = new double[m_dof_local];
			m_Ae = new double[m_dof_local * m_dof_local];
		}

		void Assembler::dof_triplet_count(void)
		{
			m_dof_triplet = 0;
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				const uint32_t nd = element->m_dof_indexes.size();
				m_dof_triplet += nd * nd;
			}
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				const uint32_t nd = constraint->m_dof_indexes.size();
				m_dof_triplet += (nd + 1) * (nd + 1);
			}
		}
		void Assembler::dof_triplet_apply(void)
		{
			//elements
			uint32_t counter = 0;
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				for(uint32_t dof_index_1 : element->m_dof_indexes)
				{
					for(uint32_t dof_index_2 : element->m_dof_indexes)
					{
						m_rows_triplet[counter] = dof_index_1;
						m_cols_triplet[counter] = dof_index_2;
						counter++;
					}
				}
			}
			//constraints
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				for(uint32_t dof_index : constraint->m_dof_indexes)
				{
					m_rows_triplet[counter] = dof_index;
					m_cols_triplet[counter] = constraint->m_dof_index;
					counter++;
				}
				for(uint32_t dof_index : constraint->m_dof_indexes)
				{
					m_cols_triplet[counter] = dof_index;
					m_rows_triplet[counter] = constraint->m_dof_index;
					counter++;
				}
				for(uint32_t dof_index_1 : constraint->m_dof_indexes)
				{
					for(uint32_t dof_index_2 : constraint->m_dof_indexes)
					{
						m_rows_triplet[counter] = dof_index_1;
						m_cols_triplet[counter] = dof_index_2;
						counter++;
					}
				}
			}
			//known dof
			const int32_t nu = m_dof_unknow;
			for(uint32_t i = 0; i < counter; i++)
			{
				if(m_rows_triplet[i] >= nu || m_cols_triplet[i] >= nu) m_rows_triplet[i] = m_cols_triplet[i] = 0;
			}
		}

		//analysis
		void Assembler::setup(void)
		{
			dof_map();
			dof_local();
			m_analysis->m_solver->m_rows_map = m_rows_map;
			m_analysis->m_solver->m_cols_map = m_cols_map;
		}

		//assemble
		void Assembler::assemble_inertia(double* M) const
		{
			//setup
			memset(M, 0, m_cols_map[m_dof_unknow] * sizeof(double));
			//elements
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->inertia(m_Ae);
				assemble_matrix(M, m_Ae, element->m_dof_indexes);
			}
		}
		void Assembler::assemble_damping(double* C) const
		{
			//setup
			memset(C, 0, m_cols_map[m_dof_unknow] * sizeof(double));
			//elements
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->damping(m_Ae);
				assemble_matrix(C, m_Ae, element->m_dof_indexes);
			}
		}
		void Assembler::assemble_stiffness(double* K) const
		{
			//setup
			memset(K, 0, m_cols_map[m_dof_unknow] * sizeof(double));
			//elements
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->stiffness(m_Ae);
				assemble_matrix(K, m_Ae, element->m_dof_indexes);
			}
		}

		void Assembler::assemble_external_force(double* fe) const
		{
			//setup
			memset(fe, 0, m_dof_unknow * sizeof(double));
		}
		void Assembler::assemble_internal_force(double* fi) const
		{
			//setup
			memset(fi, 0, m_dof_unknow * sizeof(double));
			//elements
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->internal_force(m_fe);
				assemble_vector(fi, m_fe, element->m_dof_indexes);
			}
		}

		void Assembler::assemble_vector(double* f, double* fe, const std::vector<uint32_t>& dof_indexes) const
		{
			for(uint32_t i = 0; i < dof_indexes.size(); i++)
			{
				if(dof_indexes[i] < m_dof_unknow)
				{
					f[dof_indexes[i]] += fe[i];
				}
			}
		}

		void Assembler::assemble_matrix(double* A, double* Ae, const std::vector<uint32_t>& dof_indexes) const
		{
			//data
			const uint32_t ne = dof_indexes.size();
			math::Sparse S(A, m_rows_map, m_cols_map, m_dof_unknow, m_dof_unknow);
			//assemble
			for(uint32_t i = 0; i < dof_indexes.size(); i++)
			{
				for(uint32_t j = 0; j < dof_indexes.size(); j++)
				{
					if(dof_indexes[i] < m_dof_unknow && dof_indexes[j] < m_dof_unknow)
					{
						S(dof_indexes[i], dof_indexes[j]) += Ae[i + ne * j];
					}
				}
			}
		}
		void Assembler::assemble_matrix(double* A, double* Ae, const std::vector<uint32_t>& dof_indexes, uint32_t dof_index) const
		{
			//data
			math::Sparse S(A, m_rows_map, m_cols_map, m_dof_unknow, m_dof_unknow);
			//assemble
			for(uint32_t i = 0; i < dof_indexes.size(); i++)
			{
				if(dof_indexes[i] < m_dof_unknow && dof_index < m_dof_unknow)
				{
					S(dof_indexes[i], dof_index) += Ae[i];
				}
			}
		}
		void Assembler::assemble_matrix(double* A, double* Ae, uint32_t dof_index, const std::vector<uint32_t>& dof_indexes) const
		{
			//data
			math::Sparse S(A, m_rows_map, m_cols_map, m_dof_unknow, m_dof_unknow);
			//assemble
			for(uint32_t i = 0; i < dof_indexes.size(); i++)
			{
				if(dof_index < m_dof_unknow && dof_indexes[i] < m_dof_unknow)
				{
					S(dof_index, dof_indexes[i]) += Ae[i];
				}
			}
		}

		//static
		Analysis* Assembler::m_analysis = nullptr;
	}
}
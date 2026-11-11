//std
#include <cstring>
#include <algorithm>
#include <stdexcept>

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
#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"
#include "FEA/inc/Boundary/Dependencies/Dependency.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Assembler::Assembler(void) : 
			m_rows_map{nullptr}, m_cols_map{nullptr}, m_rows_triplet{nullptr}, m_cols_triplet{nullptr},
			m_dof_know{0}, m_dof_local{0}, m_dof_total{0}, m_dof_unknow{0}, m_dof_triplet{0}, m_dof_dependent{0}
		{
			return;
		}

		//destructor
		Assembler::~Assembler(void)
		{
			delete[] m_rows_map;
			delete[] m_cols_map;
			delete[] m_rows_triplet;
			delete[] m_cols_triplet;
		}

		//data
		Compute& Assembler::compute(void)
		{
			return m_compute;
		}
		Request& Assembler::request(void)
		{
			return m_request;
		}
		uint32_t Assembler::dof_know(void) const
		{
			return m_dof_know;
		}
		uint32_t Assembler::dof_total(void) const
		{
			return m_dof_total;
		}
		uint32_t Assembler::dof_unknow(void) const
		{
			return m_dof_unknow;
		}

		//apply
		void Assembler::apply_state(const double* x) const
		{
			memcpy(m_analysis->solver()->m_x_new, x, m_dof_unknow * sizeof(double));
		}
		void Assembler::apply_velocity(const double* v) const
		{
			memcpy(m_analysis->solver()->m_v_new, v, m_dof_unknow * sizeof(double));
		}
		void Assembler::apply_acceleration(const double* a) const
		{
			memcpy(m_analysis->solver()->m_a_new, a, m_dof_unknow * sizeof(double));
		}

		//dof
		void Assembler::dof_map(void)
		{
			//mapping
			dof_triplet_count();
			delete[] m_rows_map;
			delete[] m_cols_map;
			delete[] m_rows_triplet;
			delete[] m_cols_triplet;
			int32_t* rm = m_rows_map = new int32_t[m_dof_triplet];
			int32_t* cm = m_cols_map = new int32_t[m_dof_unknow + 1];
			int32_t* rt = m_rows_triplet = new int32_t[m_dof_triplet];
			int32_t* ct = m_cols_triplet = new int32_t[m_dof_triplet];
			//sparse format
			dof_triplet_apply();
			const uint32_t nu = m_dof_unknow;
			const uint32_t nz = m_dof_triplet;
			if(umfpack_di_triplet_to_col(nu, nu, nz, rt, ct, nullptr, cm, rm, nullptr, nullptr) != UMFPACK_OK)
			{
				throw std::runtime_error("Error: Assembler sparse matrix mapping failed!\n");
			}
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
				dk.push_back(support->node()->dof_index(support->dof()));
			}
			for(const boundary::Dependency* dependency : m_analysis->m_model->m_boundary->m_dependencies)
			{
				const uint32_t d1 = dependency->node(0)->dof_index(dependency->m_dof[0]);
				const uint32_t d2 = dependency->node(1)->dof_index(dependency->m_dof[1]);
				dd.push_back(
					std::find(dk.begin(), dk.end(), d1) != dk.end() ? d2 :
					std::find(dk.begin(), dk.end(), d2) != dk.end() ? d1 : std::max(d1, d2)
				);
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
						const uint32_t i = std::distance(dd.begin(), pd);
						const uint32_t d1 = dependencies[i]->node(0)->dof_index(dependencies[i]->m_dof[0]);
						const uint32_t d2 = dependencies[i]->node(1)->dof_index(dependencies[i]->m_dof[1]);
						dof_index = dof_index == d1 ? d2 : d1;
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
				m_dof_triplet += nd * (nd + 2);
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

		//assemble
		void Assembler::assemble(void)
		{
			assemble_loads();
			assemble_elements();
			assemble_constraints();
		}
		void Assembler::assemble_loads(void)
		{
			//data
			double& V = m_analysis->solver()->m_V;
			double* fe = m_analysis->solver()->m_fe;
			double* fd = m_analysis->solver()->m_fd;
			const double t = m_analysis->solver()->m_t_new;
			const uint32_t index = m_analysis->solver()->load_combination();
			//combination
			if(index == UINT32_MAX) return;
			const boundary::LoadCombination* load_combination = m_analysis->m_model->m_boundary->m_load_combinations[index];
			//load cases
			const double p = m_analysis->solver()->m_p_new;
			for(const boundary::LoadItem* item : load_combination->load_items())
			{
				double* f = m_request.m_ft || !item->fixed() ? fe : fd;
				const boundary::LoadCase* load_case = m_analysis->m_model->m_boundary->m_load_cases[item->load_case()];
				for(const boundary::loads::Node* load : load_case->loads_nodes())
				{
					const double iv = item->value();
					const double lv = load->value();
					const double nd = load->node()->state(load->dof());
					const double Vp = m_request.m_ft || item->fixed() ? 1 : p;
					const double lt = m_request.m_ft || item->fixed() ? load->time_function(t) : 1;
					if(load->m_dof_index < m_dof_unknow)
					{
						f[load->m_dof_index] += iv * lv * lt;
						if(m_request.m_V) V -= Vp * iv * lv * lt * nd;
;					}
				}
			}
		}
		void Assembler::assemble_elements(void)
		{
			double& T = m_analysis->solver()->m_T;
			double& U = m_analysis->solver()->m_U;
			double* K = m_analysis->solver()->m_K;
			double* C = m_analysis->solver()->m_C;
			double* M = m_analysis->solver()->m_M;
			double* fi = m_analysis->solver()->m_fi;
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->compute();
			}
			for(mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->compute(m_compute);
				if(m_request.m_T) T += m_compute.m_T;
				if(m_request.m_U) U += m_compute.m_U;
				if(m_request.m_K) assemble_matrix(K, m_compute.m_K, element->m_dof_indexes);
				if(m_request.m_C) assemble_matrix(C, m_compute.m_C, element->m_dof_indexes);
				if(m_request.m_M) assemble_matrix(M, m_compute.m_M, element->m_dof_indexes);
				if(m_request.m_fi) assemble_vector(fi, m_compute.m_f, element->m_dof_indexes);
			}
		}
		void Assembler::assemble_constraints(void)
		{
			double* K = m_analysis->solver()->m_K;
			double* fi = m_analysis->solver()->m_fi;
			double* xn = m_analysis->solver()->m_x_new;
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				constraint->state(m_compute.m_x);
				double& fc = fi[constraint->m_dof_index];
				const double pc = xn[constraint->m_dof_index];
				if(m_request.m_fi)
				{
					constraint->m_function(fc, m_compute.m_x);
					constraint->m_gradient(m_compute.m_f, m_compute.m_x);
					assemble_vector(fi, m_compute.m_f, constraint->m_dof_indexes, pc);
				}
				if(m_request.m_K)
				{
					constraint->m_hessian(m_compute.m_K, m_compute.m_x);
					assemble_matrix(K, m_compute.m_K, constraint->m_dof_indexes, pc);
					assemble_matrix(K, m_compute.m_f, constraint->m_dof_indexes, constraint->m_dof_index);
					assemble_matrix(K, m_compute.m_f, constraint->m_dof_index, constraint->m_dof_indexes);
				}
			}
		}

		void Assembler::assemble_vector(double* f, const double* fe, const std::vector<uint32_t>& dof_indexes, double s) const
		{
			for(uint32_t i = 0; i < dof_indexes.size(); i++)
			{
				if(dof_indexes[i] < m_dof_unknow)
				{
					f[dof_indexes[i]] += s * fe[i];
				}
			}
		}

		void Assembler::assemble_matrix(double* A, const double* Ae, const std::vector<uint32_t>& dof_indexes, double s) const
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
						S(dof_indexes[i], dof_indexes[j]) += s * Ae[i + ne * j];
					}
				}
			}
		}
		void Assembler::assemble_matrix(double* A, const double* Ae, const std::vector<uint32_t>& dof_indexes, uint32_t dof_index, double s) const
		{
			//data
			math::Sparse S(A, m_rows_map, m_cols_map, m_dof_unknow, m_dof_unknow);
			//assemble
			for(uint32_t i = 0; i < dof_indexes.size(); i++)
			{
				if(dof_indexes[i] < m_dof_unknow && dof_index < m_dof_unknow)
				{
					S(dof_indexes[i], dof_index) += s * Ae[i];
				}
			}
		}
		void Assembler::assemble_matrix(double* A, const double* Ae, uint32_t dof_index, const std::vector<uint32_t>& dof_indexes, double s) const
		{
			//data
			math::Sparse S(A, m_rows_map, m_cols_map, m_dof_unknow, m_dof_unknow);
			//assemble
			for(uint32_t i = 0; i < dof_indexes.size(); i++)
			{
				if(dof_index < m_dof_unknow && dof_indexes[i] < m_dof_unknow)
				{
					S(dof_index, dof_indexes[i]) += s * Ae[i];
				}
			}
		}

		//analysis
		void Assembler::setup(void)
		{
			dof_map();
			dof_local();
			m_request.setup();
			m_compute.setup(m_dof_local);
			m_compute.m_request = &m_request;
			m_analysis->solver()->rows_map(m_rows_map);
			m_analysis->solver()->cols_map(m_cols_map);
		}

		//static
		Analysis* Assembler::m_analysis = nullptr;
	}
}
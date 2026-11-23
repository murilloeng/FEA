//std
#include <cstring>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <functional>

//SuiteSparse
#include <suitesparse/umfpack.h>

//Math
#include "Math/inc/Groups/ASO3.hpp"
#include "Math/inc/Groups/GSO3.hpp"
#include "Math/inc/Linear/Sparse.hpp"
#include "Math/inc/Linear/Matrix.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
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
#include "FEA/inc/Analysis/Solvers/StaticLinear.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicLinear.hpp"
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
		Data& Assembler::data(void)
		{
			return m_data;
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
			std::vector<uint32_t> dk;
			std::vector<bool> known(m_dof_total, false);
			std::vector<uint32_t> root(m_dof_total), map(m_dof_total);
			const std::function<uint32_t(uint32_t)> find = [&root](uint32_t dof){
				while(root[dof] != dof) dof = root[dof] = root[root[dof]];
				return dof;
			};
			//supports
			for(const boundary::Support* support : m_analysis->m_model->m_boundary->m_supports)
			{
				dk.push_back(support->node()->dof_index(support->dof()));
				known[dk.back()] = true;
			}
			//dependencies
			std::iota(root.begin(), root.end(), 0);
			for(const boundary::Dependency* dependency : m_analysis->m_model->m_boundary->m_dependencies)
			{
				const uint32_t r1 = find(dependency->node(0)->dof_index(dependency->m_dof[0]));
				const uint32_t r2 = find(dependency->node(1)->dof_index(dependency->m_dof[1]));
				if(r1 == r2) continue;
				if(known[r1] && known[r2])
				{
					throw std::runtime_error("Error: Dependencies link two supported dofs!");
				}
				if(known[r1] || (!known[r2] && r1 < r2)) root[r2] = r1; else root[r1] = r2;
			}
			//counters
			m_dof_dependent = 0;
			m_dof_know = (uint32_t) dk.size();
			for(uint32_t i = 0; i < m_dof_total; i++) m_dof_dependent += find(i) != i;
			m_dof_unknow = m_dof_total - m_dof_know - m_dof_dependent;
			//map
			for(uint32_t i = 0, c = 0; i < m_dof_total; i++)
			{
				if(find(i) == i && !known[i]) map[i] = c++;
			}
			for(uint32_t i = 0; i < m_dof_know; i++) map[dk[i]] = m_dof_unknow + i;
			for(uint32_t i = 0; i < m_dof_total; i++) map[i] = map[find(i)];
			//nodes
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				for(uint32_t& dof_index : node->m_dof_indexes) dof_index = map[dof_index];
			}
			//constraints
			for(boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				constraint->m_dof_index = map[constraint->m_dof_index];
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
				double* f = m_request.m_time || !item->fixed() ? fe : fd;
				const boundary::LoadCase* load_case = m_analysis->m_model->m_boundary->m_load_cases[item->load_case()];
				for(const boundary::loads::Node* load : load_case->loads_nodes())
				{
					const double iv = item->value();
					const double lv = load->value();
					const double nd = load->node()->state(load->dof());
					const double Vp = m_request.m_time || item->fixed() ? 1 : p;
					const double lt = m_request.m_time || item->fixed() ? load->time_function(t) : 1;
					const uint32_t ir = uint32_t(load->dof()) - uint32_t(mesh::nodes::DOF::Rotation_1);
					if(load->node()->m_rotation && ir < 3)
					{
						assemble_moment(f, load->node(), ir, iv * lv * lt, Vp);
					}
					else if(load->m_dof_index < m_dof_unknow)
					{
						f[load->m_dof_index] += iv * lv * lt;
						if(m_request.m_V) V -= Vp * iv * lv * lt * nd;
					}
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
			for(mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->compute(m_data);
				if(m_request.m_T) T += m_data.m_T;
				if(m_request.m_U) U += m_data.m_U;
				if(m_request.m_K) assemble_matrix(K, m_data.m_K, element->m_dof_indexes);
				if(m_request.m_C) assemble_matrix(C, m_data.m_C, element->m_dof_indexes);
				if(m_request.m_M) assemble_matrix(M, m_data.m_M, element->m_dof_indexes);
				if(m_request.m_f) assemble_vector(fi, m_data.m_f, element->m_dof_indexes);
			}
		}
		void Assembler::assemble_constraints(void)
		{
			double* M = m_analysis->solver()->m_M;
			double* C = m_analysis->solver()->m_C;
			double* K = m_analysis->solver()->m_K;
			double* f = m_analysis->solver()->m_fi;
			double* x = m_analysis->solver()->m_x_new;
			double* a = m_analysis->solver()->m_a_new;
			const double bc = boundary::Constraint::m_beta;
			const double ac = boundary::Constraint::m_alpha;
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				//data
				constraint->state(m_data.m_x);
				constraint->m_hessian(m_data.m_K, m_data.m_x);
				constraint->m_gradient(m_data.m_f, m_data.m_x);
				constraint->m_function(m_data.m_U, m_data.m_x);
				const uint32_t nd = constraint->m_dof_indexes.size();
				if(constraint->m_third) constraint->m_third(m_data.m_A, m_data.m_x);
				//assemble
				if(!m_request.m_time)
				{
					if(m_request.m_f)
					{
						f[constraint->m_dof_index] = m_data.m_U;
						assemble_vector(f, m_data.m_f, constraint->m_dof_indexes, x[constraint->m_dof_index]);
					}
					if(m_request.m_K)
					{
						assemble_matrix(K, m_data.m_f, constraint->m_dof_indexes, constraint->m_dof_index);
						assemble_matrix(K, m_data.m_f, constraint->m_dof_index, constraint->m_dof_indexes);
						assemble_matrix(K, m_data.m_K, constraint->m_dof_indexes, x[constraint->m_dof_index]);
					}
				}
				else
				{
					double gc = 0, hc = 0;
					std::vector<double> cc(nd);
					std::vector<double> kc(nd);
					constraint->velocity(m_data.m_v);
					constraint->acceleration(m_data.m_a);
					for(uint32_t i = 0; i < nd; i++)
					{
						cc[i] = 2 * ac * m_data.m_f[i];
						kc[i] = bc * bc * m_data.m_f[i];
						gc += m_data.m_f[i] * m_data.m_v[i];
						hc += m_data.m_f[i] * m_data.m_a[i];
						for(uint32_t j = 0; j < nd; j++)
						{
							hc += m_data.m_K[i + nd * j] * m_data.m_v[i] * m_data.m_v[j];
							cc[i] += (m_data.m_K[i + nd * j] + m_data.m_K[j + nd * i]) * m_data.m_v[j];
							kc[i] += m_data.m_K[j + nd * i] * (m_data.m_a[j] + 2 * ac * m_data.m_v[j]);
							for(uint32_t k = 0; k < nd; k++) kc[i] += m_data.m_A[j + nd * k + nd * nd * i] * m_data.m_v[j] * m_data.m_v[k];
						}
					}
					if(m_request.m_f)
					{
						f[constraint->m_dof_index] = hc + 2 * ac * gc + bc * bc * m_data.m_U;
						assemble_vector(f, m_data.m_f, constraint->m_dof_indexes, a[constraint->m_dof_index]);
					}
					if(m_request.m_C)
					{
						assemble_matrix(C, cc.data(), constraint->m_dof_index, constraint->m_dof_indexes);
					}
					if(m_request.m_K)
					{
						assemble_matrix(K, kc.data(), constraint->m_dof_index, constraint->m_dof_indexes);
						assemble_matrix(K, m_data.m_K, constraint->m_dof_indexes, a[constraint->m_dof_index]);
					}
					if(m_request.m_M)
					{
						assemble_matrix(M, m_data.m_f, constraint->m_dof_index, constraint->m_dof_indexes);
						assemble_matrix(M, m_data.m_f, constraint->m_dof_indexes, constraint->m_dof_index);
					}
				}
			}
		}

		void Assembler::assemble_moment(double* f, const mesh::nodes::Node* node, uint32_t index, double value, double scale)
		{
			//spatial moment on material rotation increments: f = J(t)' * Q' * m, K = -d(f) / dt
			double* K = m_analysis->solver()->m_K;
			const math::ASO3 t = node->rotation_aso3();
			const math::Mat3 J = t.jacobian();
			const math::Mat3 Q = node->rotation_gso3().matrix();
			const math::Vec3 m = value * Q.transpose() * math::Vec3(index == 0, index == 1, index == 2);
			const std::vector<uint32_t> dof_indexes = {
				node->dof_index(mesh::nodes::DOF::Rotation_1),
				node->dof_index(mesh::nodes::DOF::Rotation_2),
				node->dof_index(mesh::nodes::DOF::Rotation_3)
			};
			//force
			const math::Vec3 fm = J.transpose() * m;
			assemble_vector(f, fm.data(), dof_indexes);
			//stiffness
			const bool linear = 
				m_analysis->solver() == m_analysis->solver_static_linear() ||
				m_analysis->solver() == m_analysis->solver_dynamic_linear();
			if(m_request.m_K && !linear)
			{
				const math::Mat3 Km = t.jacobian_gradient(m) + J.transpose() * m.spin() * J;
				assemble_matrix(K, Km.data(), dof_indexes, -scale);
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
			m_data.setup(m_dof_local);
			m_data.m_request = &m_request;
			m_analysis->solver()->rows_map(m_rows_map);
			m_analysis->solver()->cols_map(m_cols_map);
		}

		//static
		Analysis* Assembler::m_analysis = nullptr;
	}
}
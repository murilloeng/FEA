//std
#include <cstring>
#include <algorithm>
#include <filesystem>

//umfpack
#include <suitesparse/umfpack.h>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Linear/Quat.hpp"
#include "Math/inc/Linear/Mat3.hpp"
#include "Math/inc/Linear/Sparse.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Joints/Joint.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Time/Time.hpp"
#include "FEA/inc/Boundary/Loads/Item.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Initials/Initial.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"
#include "FEA/inc/Boundary/Loads/Nodes/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Element.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"
#include "FEA/inc/Boundary/Dependencies/Dependency.hpp"

#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Time.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Assembler::Assembler(void)
		{
			setup(false);
		}

		//destructor
		Assembler::~Assembler(void)
		{
			delete[] m_de;
			delete[] m_ve;
			delete[] m_ae;
			delete[] m_fe;
			delete[] m_Ke;
			delete[] m_Ce;
			delete[] m_Me;
			delete[] m_rows_map;
			delete[] m_cols_map;
			delete[] m_rows_triplet;
			delete[] m_cols_triplet;
		}

		//serialization
		void Assembler::load_results(void)
		{
			m_analysis->load_results();
			m_analysis->m_model->m_mesh->load_results();
			m_analysis->m_model->m_boundary->load_results();
		}
		void Assembler::save_results(void) const
		{
			//data
			const std::string directory = m_analysis->m_model->folder();
			const uint32_t solver_set = m_analysis->m_solver->state_set();
			const std::string state_name[] = {"State", "Velocity", "Acceleration"};
			const uint32_t state_set[] = {uint32_t(solvers::state::x), uint32_t(solvers::state::v), uint32_t(solvers::state::a)};
			//setup
			for(uint32_t i = 0; i < 3; i++)
			{
				if(solver_set & state_set[i])
				{
					std::filesystem::remove_all(directory + "/" + state_name[i]);
					std::filesystem::create_directories(directory + "/" + state_name[i]);
				}
			}
			std::filesystem::remove_all(directory + "/Energy");
			std::filesystem::remove_all(directory + "/Solver");
			std::filesystem::remove_all(directory + "/Joints");
			std::filesystem::remove_all(directory + "/Supports");
			std::filesystem::remove_all(directory + "/Elements");
			std::filesystem::create_directories(directory + "/Energy");
			std::filesystem::create_directories(directory + "/Solver");
			std::filesystem::create_directories(directory + "/Joints");
			std::filesystem::create_directories(directory + "/Supports");
			std::filesystem::create_directories(directory + "/Elements");
			m_analysis->m_model->m_results->m_steps = m_analysis->m_solver->m_step;
			//save
			m_analysis->save_results();
			m_analysis->m_model->m_mesh->save_results();
			m_analysis->m_model->m_boundary->save_results();
		}

		//dof
		void Assembler::map_dof(void)
		{
			//map dof
			count_dof();
			m_rows_triplet = new int32_t[m_dof_triplet];
			m_cols_triplet = new int32_t[m_dof_triplet];
			m_rows_map = new int32_t[m_dof_triplet];
			m_cols_map = new int32_t[m_dof_unknow + 1];
			memset(m_cols_map, 0, (m_dof_unknow + 1) * sizeof(int32_t));
			for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				add_dof(joint->m_dof_index);
			}
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				add_dof(element->m_dof_index);
			}
			for(const boundary::Support* support : m_analysis->m_model->m_boundary->m_supports)
			{
				add_dof(support->m_dof_index, support->m_dof_index);
			}
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				add_dof(constraint->dof_list());
			}
			//sparse format
			const uint32_t nu = m_dof_unknow;
			const uint32_t nz = m_cols_map[m_dof_unknow];
			umfpack_di_triplet_to_col(nu, nu, nz, m_rows_triplet, m_cols_triplet, nullptr, m_cols_map, m_rows_map, nullptr, nullptr);
		}

		void Assembler::count_dof(void)
		{
			for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				count_dof(joint->m_dof_index);
			}
			for(const boundary::Support* support : m_analysis->m_model->m_boundary->m_supports)
			{
				count_dof(support->m_dof_index);
			}
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				count_dof(element->m_dof_index);
			}
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				count_dof(constraint->dof_list());
			}
		}
		void Assembler::count_dof(uint32_t i)
		{
			uint32_t p = 0;
			if(i < m_dof_unknow) p++;
			m_dof_triplet += p * p;
		}
		void Assembler::count_dof(const ulist& list)
		{
			uint32_t p = 0;
			for(uint32_t i : list)
			{
				if(i < m_dof_unknow)
				{
					p++;
				}
			}
			m_dof_triplet += p * p;
		}

		void Assembler::add_dof(const ulist& list)
		{
			for(uint32_t index_1 : list)
			{
				for(uint32_t index_2 : list)
				{
					add_dof(index_1, index_2);
				}
			}
		}
		void Assembler::add_dof(uint32_t i, uint32_t j)
		{
			int32_t& k = m_cols_map[m_dof_unknow];
			if(i < m_dof_unknow && j < m_dof_unknow)
			{
				m_rows_triplet[k] = i;
				m_cols_triplet[k++] = j;
			}
		}

		void Assembler::sort_dof(void)
		{
			//dof lists
			ulist dd, dk;
			sort_dof_lists(dd, dk);
			//dof sizes
			m_dof_know = (uint32_t) dk.size();
			m_dof_dependent = (uint32_t) dd.size();
			m_dof_unknow = m_dof_total - m_dof_know - m_dof_dependent;
			//dof sorting
			sort_dof_nodes(dd, dk);
			sort_dof_constraints();
		}
		void Assembler::save_dof(void)
		{
			//save dof types
			std::string buffer;
			char formatter[200];
			for(const mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				sprintf(formatter, "%03d ", node->m_dof_set);
				buffer += formatter;
				for(uint32_t dof_index : node->m_dof_index)
				{
					sprintf(formatter, "%05d ", dof_index);
					buffer += formatter;
				}
				buffer += "\n";
			}
			//write in file
			std::filesystem::create_directories(m_analysis->m_model->folder() + "/Assembler/");
			FILE* file = fopen((m_analysis->m_model->folder() + "/Assembler/Dof.txt").c_str(), "w");
			fprintf(file, "%s", buffer.c_str());
			fclose(file);
		}
		void Assembler::apply_dof(void)
		{
			//clear
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->m_dof_set = 0;
			}
			//apply
			m_analysis->apply_dof();
			m_analysis->m_model->m_mesh->apply_dof();
			m_analysis->m_model->m_boundary->apply_dof();
		}
		void Assembler::setup_dof(void)
		{
			m_dof_total = 0;
			m_analysis->m_model->m_mesh->setup_dof(m_dof_total);
			m_analysis->m_model->m_boundary->setup_dof(m_dof_total);
		}
		void Assembler::sort_dof_lists(ulist& dd, ulist& dk) const
		{
			for(const boundary::Support* support : m_analysis->m_model->m_boundary->m_supports)
			{
				const mesh::nodes::dof dof = support->m_dof;
				const mesh::nodes::Node* node = support->node();
				dk.push_back(node->dof_index(dof));
			}
			for(const boundary::Dependency* dependency : m_analysis->m_model->m_boundary->m_dependencies)
			{
				dd.push_back(dependency->dof_index(true));
			}
		}
		void Assembler::sort_dof_nodes(const ulist& dd, const ulist& dk) const
		{
			//data
			uint32_t cd = 0, ck = 0;
			const uint32_t nu = m_dof_unknow;
			const std::vector<boundary::Dependency*> dependencies = m_analysis->m_model->m_boundary->m_dependencies;
			//sort
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				for(uint32_t& dof_index : node->m_dof_index)
				{
					ulist::const_iterator pd = std::find(dd.begin(), dd.end(), dof_index);
					ulist::const_iterator pk = std::find(dk.begin(), dk.end(), dof_index);
					if(pd != dd.end() ? ++cd : false)
					{
						dof_index = dependencies[std::distance(dd.begin(), pd)]->dof_index(0);
					}
					else if(pk != dk.end() ? ++ck : false)
					{
						dof_index = nu + (uint32_t) std::distance(dk.begin(), pk);
					}
					else
					{
						dof_index -= cd + ck;
					}
				}
			}
		}
		void Assembler::sort_dof_constraints(void) const
		{
			for(boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				constraint->m_dof_index -= m_dof_know + m_dof_dependent;
			}
		}

		//memory
		void Assembler::setup(bool clean)
		{
			//dof
			if(clean) cleanup();
			m_dof_know = m_dof_local = 0;
			m_dof_total = m_dof_unknow = 0;
			m_dof_triplet = 0;
			//local
			m_qe = 0;
			m_de = m_ve = m_ae = nullptr;
			m_fe = m_Ke = m_Ce = m_Me = nullptr;
			//dofs
			m_cols_map = m_rows_map = nullptr;
			m_cols_triplet = m_rows_triplet = nullptr;
		}
		void Assembler::cleanup(void)
		{
			delete[] m_de;
			delete[] m_ve;
			delete[] m_ae;
			delete[] m_fe;
			delete[] m_Ke;
			delete[] m_Ce;
			delete[] m_Me;
			delete[] m_cols_map;
			delete[] m_rows_map;
			delete[] m_cols_triplet;
			delete[] m_rows_triplet;
		}
		void Assembler::allocate(void)
		{
			//dof
			map_dof();
			uint32_t& nd = m_dof_local = 0;
			const uint32_t nk = m_dof_know;
			const uint32_t nu = m_dof_unknow;
			const uint32_t nz = m_cols_map[nu];
			//allocate
			m_solver->cleanup();
			m_solver->allocate(nu, nk, nz);
			for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				nd = std::max(nd, (uint32_t) joint->m_dof_index.size());
			}
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				nd = std::max(nd, (uint32_t) element->m_dof_index.size());
			}
			m_de = new double[nd];
			m_ve = new double[nd];
			m_ae = new double[nd];
			m_fe = new double[nd];
			m_Ke = new double[nd * nd];
			m_Ce = new double[nd * nd];
			m_Me = new double[nd * nd];
		}

		//analysis
		void Assembler::setup(void)
		{
			//setup
			setup(true);
			apply_dof();
			setup_dof();
			sort_dof();
			save_dof();
			m_analysis->setup();
			m_analysis->m_model->m_mesh->setup();
			m_analysis->m_model->m_boundary->setup();
			//allocate
			allocate();
			//mesh report
			printf("-------------------------------------------------------------------------------\n");
			printf("Mesh:\n");
			printf("\tNodes:\t\t %d\n", (uint32_t) m_analysis->model()->mesh()->nodes().size());
			printf("\tElements:\t %d\n", (uint32_t) m_analysis->model()->mesh()->elements().size());
			printf("-------------------------------------------------------------------------------\n");
			//dof report
			printf("-------------------------------------------------------------------------------\n");
			printf("Degrees of freedom:\n");
			printf("\tKnow:\t\t %d\n", m_dof_know);
			printf("\tUnknow:\t\t %d\n", m_dof_unknow);
			printf("\tDependent:\t %d\n", m_dof_dependent);
			printf("\tTotal:\t\t %d\n", m_dof_total);
			printf("-------------------------------------------------------------------------------\n");
		}
		void Assembler::check(void) const
		{
			//print
			printf("-------------------------------------------------------------------------------\n");
			printf("Checking model:\n");
			//check mesh
			printf("\tMesh: ");
			m_analysis->m_model->m_mesh->check(), printf("OK\n");
			//check analysis
			printf("\tSolver: ");
			m_analysis->check(), printf("OK\n");
			//check boundary
			printf("\tBoundary: ");
			m_analysis->m_model->m_boundary->check(), printf("OK\n");
			printf("-------------------------------------------------------------------------------\n");
		}
		void Assembler::finish(void) const
		{
			//data
			const uint32_t solver_set = m_analysis->m_solver->state_set();
			const std::string state_name[] = {"State", "Velocity", "Acceleration"};
			const std::string path = m_analysis->m_model->m_path + std::string("/") + m_analysis->m_model->m_name;
			const uint32_t state_set[] = {uint32_t(solvers::state::x), uint32_t(solvers::state::v), uint32_t(solvers::state::a)};
			//setup
			std::filesystem::create_directories(path);
			for(uint32_t i = 0; i < 3; i++)
			{
				if(solver_set & state_set[i])
				{
					std::filesystem::remove_all(path + "/" + state_name[i]);
					std::filesystem::create_directories(path + "/" + state_name[i]);
				}
			}
			std::filesystem::remove_all(path + "/Solver");
			std::filesystem::remove_all(path + "/Joints");
			std::filesystem::remove_all(path + "/Supports");
			std::filesystem::remove_all(path + "/Elements");
			std::filesystem::create_directories(path + "/Solver");
			std::filesystem::create_directories(path + "/Joints");
			std::filesystem::create_directories(path + "/Supports");
			std::filesystem::create_directories(path + "/Elements");
			m_analysis->m_model->m_results->m_steps = m_analysis->m_solver->m_step;
			//energy
			m_analysis->m_model->save_results();
		}

		void Assembler::record(void) const
		{
			m_analysis->m_solver->record();
			m_analysis->m_model->m_mesh->record();
			m_analysis->m_model->m_boundary->record();
		}
		void Assembler::update(void) const
		{
			m_analysis->update();
			m_analysis->m_model->m_mesh->update();
			m_analysis->m_model->m_boundary->update();
		}
		void Assembler::restore(void) const
		{
			m_analysis->restore();
			m_analysis->m_model->m_mesh->restore();
			m_analysis->m_model->m_boundary->restore();
		}

		//dof
		uint32_t Assembler::dof_know(void) const
		{
			return m_dof_know;
		}
		uint32_t Assembler::dof_local(void) const
		{
			return m_dof_local;
		}
		uint32_t Assembler::dof_total(void) const
		{
			return m_dof_total;
		}
		uint32_t Assembler::dof_unknow(void) const
		{
			return m_dof_unknow;
		}
		uint32_t Assembler::dof_triplet(void) const
		{
			return m_dof_triplet;
		}
		uint32_t Assembler::dof_nonzero(void) const
		{
			return m_cols_map[m_dof_unknow];
		}

		//map
		const int32_t* Assembler::rows_map(void) const
		{
			return m_rows_map;
		}
		const int32_t* Assembler::cols_map(void) const
		{
			return m_cols_map;
		}
		const int32_t* Assembler::rows_triplet(void) const
		{
			return m_rows_triplet;
		}
		const int32_t* Assembler::cols_triplet(void) const
		{
			return m_cols_triplet;
		}

		//assemble
		void Assembler::assemble(void) const
		{
			assemble_forces();
			assemble_tangents();
		}
		void Assembler::assemble_forces(void) const
		{
			//data
			const uint32_t fs = m_analysis->m_solver->force_set();
			//assemble
			if(fs & uint32_t(solvers::force::fd)) assemble_dead_force();
			if(fs & uint32_t(solvers::force::fe)) assemble_external_force();
			if(fs & uint32_t(solvers::force::fn)) assemble_inertial_force();
			if(fs & uint32_t(solvers::force::fi)) assemble_internal_force();
			if(fs & uint32_t(solvers::force::fr)) assemble_reference_force();
		}
		void Assembler::assemble_tangents(void) const
		{
			//data
			const uint32_t ts = m_analysis->m_solver->tangent_set();
			//assemble
			if(ts & uint32_t(solvers::tangent::M)) assemble_inertia();
			if(ts & uint32_t(solvers::tangent::C)) assemble_damping();
			if(ts & uint32_t(solvers::tangent::K)) assemble_stiffness();
		}

		void Assembler::assemble_state(void) const
		{
			double* u = m_analysis->m_solver->m_x_new;
			for(const mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				for(const uint32_t& dof_index : node->m_dof_index)
				{
					if(dof_index < m_dof_unknow)
					{
						u[dof_index] = node->m_state_new[&dof_index - &node->m_dof_index[0]];
					}
				}
			}
		}
		void Assembler::assemble_velocity(void) const
		{
			double* v = m_analysis->m_solver->m_v_new;
			for(const mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				for(const uint32_t& dof_index : node->m_dof_index)
				{
					if(dof_index < m_dof_unknow)
					{
						v[dof_index] = node->m_velocity_new[&dof_index - &node->m_dof_index[0]];
					}
				}
			}
		}
		void Assembler::assemble_acceleration(void) const
		{
			double* a = m_analysis->m_solver->m_a_new;
			for(const mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				for(const uint32_t& dof_index : node->m_dof_index)
				{
					if(dof_index < m_dof_unknow)
					{
						a[dof_index] = node->m_acceleration_new[&dof_index - &node->m_dof_index[0]];
					}
				}
			}
		}

		void Assembler::assemble_inertial_force(void) const
		{
			//data
			double* fu = m_analysis->m_solver->m_fnu;
			double* fk = m_analysis->m_solver->m_fnk;
			//clear
			memset(fk, 0, m_dof_know * sizeof(double));
			memset(fu, 0, m_dof_unknow * sizeof(double));
			//assemble
			assemble_inertial_force_joints();
			assemble_inertial_force_elements();
			assemble_inertial_force_constraints();
		}
		void Assembler::assemble_internal_force(void) const
		{
			//data
			double* fu = m_analysis->m_solver->m_fiu;
			double* fk = m_analysis->m_solver->m_fik;
			//clear
			memset(fk, 0, m_dof_know * sizeof(double));
			memset(fu, 0, m_dof_unknow * sizeof(double));
			//assemble
			assemble_internal_force_joints();
			assemble_internal_force_elements();
			assemble_internal_force_constraints();
		}

		void Assembler::assemble_kinetic_energy(void) const
		{
			//data
			const uint32_t step = m_analysis->m_solver->m_step;
			double& K = m_analysis->m_solver->m_energy_data[2 * step + 0] = 0;
			//assemble
			for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				K += joint->m_energy_data[2 * step + 0] = joint->kinetic_energy();
			}
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				K += element->m_energy_data[2 * step + 0] = element->kinetic_energy();
			}
		}
		void Assembler::assemble_internal_energy(void) const
		{
			//data
			const uint32_t step = m_analysis->m_solver->m_step;
			double& U = m_analysis->m_solver->m_energy_data[2 * step + 1] = 0;
			//assemble
			for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				U += joint->m_energy_data[2 * step + 1] = joint->internal_energy();
			}
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				U += element->m_energy_data[2 * step + 1] = element->internal_energy();
			}
		}

		void Assembler::assemble_dead_force(void) const
		{
			//data
			double* fu = m_analysis->m_solver->m_fdu;
			double* fk = m_analysis->m_solver->m_fdk;
			//clear
			memset(fk, 0, m_dof_know * sizeof(double));
			memset(fu, 0, m_dof_unknow * sizeof(double));
			//assemble
			if(m_analysis->m_solver->m_combination != UINT_MAX)
			{
				assemble_dead_force_nodes();
				assemble_dead_force_elements();
			}
		}
		void Assembler::assemble_external_force(void) const
		{
			//data
			double* fu = m_analysis->m_solver->m_feu;
			double* fk = m_analysis->m_solver->m_fek;
			const double t = m_analysis->m_solver->m_t_new;
			//clear
			memset(fk, 0, m_dof_know * sizeof(double));
			memset(fu, 0, m_dof_unknow * sizeof(double));
			//assemble
			if(m_analysis->m_solver->m_combination != UINT_MAX)
			{
				assemble_external_force_nodes(t);
				assemble_external_force_elements(t);
			}
		}
		void Assembler::assemble_reference_force(void) const
		{
			//data
			double* fu = m_analysis->m_solver->m_fru;
			double* fk = m_analysis->m_solver->m_frk;
			//clear
			memset(fk, 0, m_dof_know * sizeof(double));
			memset(fu, 0, m_dof_unknow * sizeof(double));
			//assemble
			if(m_analysis->m_solver->m_combination != UINT_MAX)
			{
				assemble_reference_force_nodes();
				assemble_reference_force_elements();
			}
		}

		void Assembler::assemble_inertia(void) const
		{
			//data
			double* M = m_analysis->m_solver->m_M;
			//clear
			memset(M, 0, m_cols_map[m_dof_unknow] * sizeof(double));
			//assemble
			assemble_inertia_elements();
			assemble_inertia_constraints();
		}
		void Assembler::assemble_damping(void) const
		{
			//data
			double* C = m_analysis->m_solver->m_C;
			//clear
			memset(C, 0, m_cols_map[m_dof_unknow] * sizeof(double));
			//assemble
			assemble_damping_elements();
			assemble_damping_constraints();
		}
		void Assembler::assemble_stiffness(void) const
		{
			//data
			double* K = m_analysis->m_solver->m_K;
			//clear
			memset(K, 0, m_cols_map[m_dof_unknow] * sizeof(double));
			//assemble
			assemble_stiffness_joints();
			assemble_stiffness_elements();
			assemble_stiffness_constraints();
		}

		void Assembler::assemble_state_increment(void) const
		{
			uint32_t p;
			double* du = m_analysis->m_solver->m_dx;
			for(const mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				for(const uint32_t& dof_index : node->m_dof_index)
				{
					if(dof_index < m_dof_unknow)
					{
						p = &dof_index - &node->m_dof_index[0];
						du[dof_index] = node->m_state_new[p] - node->m_state_old[p];
					}
				}
			}
		}
		void Assembler::assemble_velocity_increment(void) const
		{
			uint32_t p;
			double* dv = m_analysis->m_solver->m_dv;
			for(const mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				for(const uint32_t& dof_index : node->m_dof_index)
				{
					if(dof_index < m_dof_unknow)
					{
						p = &dof_index - &node->m_dof_index[0];
						dv[dof_index] = node->m_velocity_new[p] - node->m_velocity_old[p];
					}
				}
			}
		}
		void Assembler::assemble_acceleration_increment(void) const
		{
			uint32_t p;
			double* da = m_analysis->m_solver->m_da;
			for(const mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				for(const uint32_t& dof_index : node->m_dof_index)
				{
					if(dof_index < m_dof_unknow)
					{
						p = &dof_index - &node->m_dof_index[0];
						da[dof_index] = node->m_acceleration_new[p] - node->m_acceleration_old[p];
					}
				}
			}
		}

		//apply
		void Assembler::apply(void) const
		{
			//data
			static double t = 0;
			const double t_new = m_solver->m_t_new;
			const uint32_t step = m_solver->m_step;
			const uint32_t ss = m_solver->state_set();
			//boundary
			if(step == 0) apply_initials();
			if(step == 0 || t != t_new) t = t_new, apply_supports();
			//mesh
			if(ss & uint32_t(solvers::state::x)) apply_state();
			if(ss & uint32_t(solvers::state::v)) apply_velocity();
			if(ss & uint32_t(solvers::state::a)) apply_acceleration();
		}
		void Assembler::apply_state(void) const
		{
			//data
			const uint32_t ss = m_analysis->m_solver->state_set();
			//apply
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->apply_state();
			}
			if(~ss & uint32_t(solvers::state::v) && ~ss & uint32_t(solvers::state::a))
			{
				for(boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
				{
					constraint->apply_state();
				}
			}
		}
		void Assembler::apply_velocity(void) const
		{
			//data
			const uint32_t ss = m_analysis->m_solver->state_set();
			//apply
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->apply_velocity();
			}
			if(ss & uint32_t(solvers::state::v) && ~ss & uint32_t(solvers::state::a))
			{
				for(boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
				{
					constraint->apply_state();
				}
			}
		}
		void Assembler::apply_initials(void) const
		{
			for(const boundary::Initial* initial : m_analysis->m_model->m_boundary->m_initials)
			{
				initial->apply();
			}
		}
		void Assembler::apply_supports(void) const
		{
			for(const boundary::Support* support : m_analysis->m_model->m_boundary->m_supports)
			{
				support->apply();
			}
			for(uint32_t i : boundary::Support::m_update_nodes)
			{
				m_analysis->m_model->m_mesh->m_nodes[i]->update_rotation();
			}
		}
		void Assembler::apply_acceleration(void) const
		{
			//data
			const uint32_t ss = m_analysis->m_solver->state_set();
			//apply
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->apply_acceleration();
			}
			if(ss & uint32_t(solvers::state::a))
			{
				for(boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
				{
					constraint->apply_acceleration();
				}
			}
		}

		//compute
		void Assembler::compute(void) const
		{
			compute_joints();
			compute_elements();
			compute_constraints();
		}
		void Assembler::compute_joints(void) const
		{
			for(mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				joint->compute();
			}
		}
		void Assembler::compute_elements(void) const
		{
			for(mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->compute();
			}
		}
		void Assembler::compute_constraints(void) const
		{
			for(boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				constraint->compute();
			}
		}

		//increment
		void Assembler::increment_state(void) const
		{
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->increment_state();
			}
			for(boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				constraint->increment_state();
			}
		}
		void Assembler::increment_velocity(void) const
		{
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->increment_velocity();
			}
		}
		void Assembler::increment_acceleration(void) const
		{
			for(mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				node->increment_acceleration();
			}
		}

		//adjust
		void Assembler::adjust_moments(double* fu, double* fk, double* K, double p) const
		{
			const uint32_t nu = m_dof_unknow;
			const uint32_t t1 = uint32_t(mesh::nodes::dof::rotation_1);
			for(const mesh::nodes::Node* node : m_analysis->m_model->m_mesh->m_nodes)
			{
				//data
				if(!node->m_quat_new) continue;
				const uint32_t index = math::bit_index(node->m_dof_set, t1);
				//list
				const ulist dl = {
					node->m_dof_index[index + 0],
					node->m_dof_index[index + 1],
					node->m_dof_index[index + 2]
				};
				//moment
				const math::Vec3 ms = {
					dl[0] < nu ? fu[dl[0]] : fk[dl[0] - nu],
					dl[1] < nu ? fu[dl[1]] : fk[dl[1] - nu],
					dl[2] < nu ? fu[dl[2]] : fk[dl[2] - nu]
				};
				//state
				if(ms.norm() == 0) continue;
				const math::Vec3 t_new = node->m_state_new + index;
				//parametrization
				const math::Vec3 mt = t_new.rotation_gradient(ms, true);
				const math::Mat3 Ht = t_new.rotation_hessian(-p * ms, true);
				//assemble
				for(uint32_t i = 0; i < 3; i++)
				{
					(dl[i] < nu ? fu[dl[i]] : fk[dl[i] - nu]) = mt[i];
				}
				assemble_matrix(Ht.data(), dl, K);
			}
		}

		//assemble
		void Assembler::assemble_number(double v, uint32_t d, double* fu, double* fk) const
		{
			(d < m_dof_unknow ? fu[d] : fk[d - m_dof_unknow]) += v;
		}
		void Assembler::assemble_number(double v, uint32_t pi, uint32_t pj, double* K) const
		{
			//data
			const uint32_t nu = m_dof_unknow;
			math::Sparse Km(K, m_rows_map, m_cols_map, nu, nu);
			//assemble
			if(pi < nu && pj < nu) Km(pi, pj) += v;
		}
		void Assembler::assemble_matrix(const double* k, const ulist& d, double* K, double s) const
		{
			const uint32_t ne = d.size();
			for(uint32_t i = 0; i < ne; i++)
			{
				for(uint32_t j = 0; j < ne; j++)
				{
					assemble_number(s * k[i + ne * j], d[i], d[j], K);
				}
			}
		}
		void Assembler::assemble_vector(const double* f, const ulist& d, double* fu, double* fk, double s) const
		{
			for(uint32_t i = 0; i < d.size(); i++)
			{
				assemble_number(s * f[i], d[i], fu, fk);
			}
		}
		void Assembler::assemble_vector(const double* f, const ulist& dr, uint32_t dc, double* K, double s) const
		{
			for(uint32_t i = 0; i < dr.size(); i++)
			{
				assemble_number(s * f[i], dr[i], dc, K);
			}
		}
		void Assembler::assemble_vector(const double* f, uint32_t dr, const ulist& dc, double* K, double s) const
		{
			for(uint32_t i = 0; i < dc.size(); i++)
			{
				assemble_number(s * f[i], dr, dc[i], K);
			}
		}

		void Assembler::assemble_dead_force_nodes(void) const
		{
			double* fu = m_analysis->m_solver->m_fdu;
			double* fk = m_analysis->m_solver->m_fdk;
			const double t = m_analysis->m_solver->m_t_new;
			const uint32_t lc = m_analysis->m_solver->m_combination;
			const boundary::loads::LoadCombination* combination = m_analysis->m_model->m_boundary->m_load_combinations[lc];
			for(const boundary::loads::Item* item : combination->m_items)
			{
				if(item->fixed())
				{
					//load case
					const double v = item->value();
					const uint32_t a = item->load_case();
					const boundary::loads::LoadCase* load_case = m_analysis->m_model->m_boundary->m_load_cases[a];
					//loads loop
					for(const boundary::loads::Node* load : load_case->m_loads_nodes)
					{
						//load
						const double p = load->m_value;
						const double f = load->time_value(t);
						const uint32_t d = load->m_dof_index;
						//assemble
						assemble_number(v * f * p, d, fu, fk);
					}
				}
			}
		}
		void Assembler::assemble_dead_force_elements(void) const
		{
			double* fu = m_analysis->m_solver->m_fdu;
			double* fk = m_analysis->m_solver->m_fdk;
			const double t = m_analysis->m_solver->m_t_new;
			const uint32_t lc = m_analysis->m_solver->m_combination;
			const boundary::loads::LoadCombination* combination = m_analysis->m_model->m_boundary->m_load_combinations[lc];
			for(const boundary::loads::Item* item : combination->m_items)
			{
				if(item->fixed())
				{
					//load case
					const double v = item->value();
					const uint32_t a = item->load_case();
					const boundary::loads::LoadCase* load_case = m_analysis->m_model->m_boundary->m_load_cases[a];
					//loads loop
					for(const boundary::loads::Element* load : load_case->m_loads_elements)
					{
						const double f = load->time_value(t);
						load->element()->reference_force(m_fe, load);
						assemble_vector(m_fe, load->element()->m_dof_index, fu, fk, v * f);
					}
				}
			}
		}

		void Assembler::assemble_inertial_force_joints(void) const
		{
			double* fu = m_analysis->m_solver->m_fnu;
			double* fk = m_analysis->m_solver->m_fnk;
			for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				joint->inertial_force(m_fe);
				assemble_vector(m_fe, joint->m_dof_index, fu, fk);
			}
		}
		void Assembler::assemble_inertial_force_elements(void) const
		{
			double* fu = m_analysis->m_solver->m_fnu;
			double* fk = m_analysis->m_solver->m_fnk;
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->inertial_force(m_fe);
				assemble_vector(m_fe, element->m_dof_index, fu, fk);
			}
		}
		void Assembler::assemble_inertial_force_constraints(void) const
		{
			//data
			double* fu = m_analysis->m_solver->m_fnu;
			double* fk = m_analysis->m_solver->m_fnk;
			//constraints
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				//data
				const uint32_t nd = constraint->m_dof.size();
				const math::Vector cg(constraint->m_gradient_data, nd);
				const math::Vector ca(constraint->m_dof_acceleration_data, nd);
				//assemble
				assemble_number(cg.inner(ca), constraint->m_dof_index, fu, fk);
			}
		}

		void Assembler::assemble_internal_force_joints(void) const
		{
			double* fu = m_analysis->m_solver->m_fiu;
			double* fk = m_analysis->m_solver->m_fik;
			for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				joint->internal_force(m_fe);
				assemble_vector(m_fe, joint->m_dof_index, fu, fk);
			}
		}
		void Assembler::assemble_internal_force_elements(void) const
		{
			double* fu = m_analysis->m_solver->m_fiu;
			double* fk = m_analysis->m_solver->m_fik;
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->internal_force(m_fe);
				assemble_vector(m_fe, element->m_dof_index, fu, fk);
			}
		}
		void Assembler::assemble_internal_force_constraints(void) const
		{
			//data
			double* fu = m_analysis->m_solver->m_fiu;
			double* fk = m_analysis->m_solver->m_fik;
			const uint32_t ss = m_analysis->m_solver->state_set();
			const double b = boundary::Constraint::m_baumgarte.beta();
			const double a = boundary::Constraint::m_baumgarte.alpha();
			//constraints
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				//data
				const double cp = constraint->m_state_new;
				const double cf = constraint->m_function_data;
				const uint32_t nd = constraint->m_dof_indexes.size();
				const math::Vector cg(constraint->m_gradient_data, nd);
				const math::Matrix ch(constraint->m_hessian_data, nd, nd);
				const math::Vector cv(constraint->m_dof_velocity_data, nd);
				//assemble
				assemble_number(b * b * cf, constraint->m_dof_index, fu, fk);
				assemble_vector(cg.data(), constraint->m_dof_indexes, fu, fk, cp);
				if(ss & uint32_t(solvers::state::v))
				{
					assemble_number(ch.bilinear(cv), constraint->m_dof_index, fu, fk);
					assemble_number(2 * a * cg.inner(cv), constraint->m_dof_index, fu, fk);
				}
			}
		}

		void Assembler::assemble_external_force_nodes(double t) const
		{
			double* fu = m_analysis->m_solver->m_feu;
			double* fk = m_analysis->m_solver->m_fek;
			const uint32_t lc = m_analysis->m_solver->m_combination;
			const boundary::loads::LoadCombination* combination = m_analysis->m_model->m_boundary->m_load_combinations[lc];
			for(const boundary::loads::Item* item : combination->m_items)
			{
				if(!item->fixed())
				{
					//load case
					const double v = item->value();
					const uint32_t a = item->load_case();
					const boundary::loads::LoadCase* load_case = m_analysis->m_model->m_boundary->m_load_cases[a];
					//loads loop
					for(const boundary::loads::Node* load : load_case->m_loads_nodes)
					{
						//load
						const double p = load->m_value;
						const double f = load->time_value(t);
						const uint32_t d = load->m_dof_index;
						//assemble
						assemble_number(v * f * p, d, fu, fk);
					}
				}
			}
		}
		void Assembler::assemble_external_force_elements(double t) const
		{
			double* fu = m_analysis->m_solver->m_feu;
			double* fk = m_analysis->m_solver->m_fek;
			const uint32_t lc = m_analysis->m_solver->m_combination;
			const boundary::loads::LoadCombination* combination = m_analysis->m_model->m_boundary->m_load_combinations[lc];
			for(const boundary::loads::Item* item : combination->m_items)
			{
				if(!item->fixed())
				{
					//load case
					const double v = item->value();
					const uint32_t a = item->load_case();
					const boundary::loads::LoadCase* load_case = m_analysis->m_model->m_boundary->m_load_cases[a];
					//loads loop
					for(const boundary::loads::Element* load : load_case->m_loads_elements)
					{
						const double f = load->time_value(t);
						load->element()->reference_force(m_fe, load);
						assemble_vector(m_fe, load->element()->m_dof_index, fu, fk, v * f);
					}
				}
			}
		}

		void Assembler::assemble_reference_force_nodes(void) const
		{
			double* K = m_analysis->m_solver->m_K;
			double* fu = m_analysis->m_solver->m_fru;
			double* fk = m_analysis->m_solver->m_frk;
			const uint32_t lc = m_analysis->m_solver->m_combination;
			const boundary::loads::LoadCombination* combination = m_analysis->m_model->m_boundary->m_load_combinations[lc];
			for(const boundary::loads::Item* item : combination->m_items)
			{
				if(!item->fixed())
				{
					//load case
					const double v = item->value();
					const uint32_t a = item->load_case();
					const boundary::loads::LoadCase* load_case = m_analysis->m_model->m_boundary->m_load_cases[a];
					//loads loop
					for(const boundary::loads::Node* load : load_case->m_loads_nodes)
					{
						//load
						const double p = load->m_value;
						const uint32_t d = load->m_dof_index;
						//assemble
						assemble_number(v * p, d, fu, fk);
					}
				}
			}
			adjust_moments(fu, fk, K, m_solver->m_p_new);
		}
		void Assembler::assemble_reference_force_elements(void) const
		{
			double* fu = m_analysis->m_solver->m_fru;
			double* fk = m_analysis->m_solver->m_frk;
			const uint32_t lc = m_analysis->m_solver->m_combination;
			const boundary::loads::LoadCombination* combination = m_analysis->m_model->m_boundary->m_load_combinations[lc];
			for(const boundary::loads::Item* item : combination->m_items)
			{
				if(!item->fixed())
				{
					//load case
					const double v = item->value();
					const uint32_t a = item->load_case();
					const boundary::loads::LoadCase* load_case = m_analysis->m_model->m_boundary->m_load_cases[a];
					//loads loop
					for(const boundary::loads::Element* load : load_case->m_loads_elements)
					{
						load->element()->reference_force(m_fe, load);
						assemble_vector(m_fe, load->element()->m_dof_index, fu, fk, v);
					}
				}
			}
		}

		void Assembler::assemble_inertia_joints(void) const
		{
			double* M = m_analysis->m_solver->m_M;
			for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				joint->inertia(m_Me);
				assemble_matrix(m_Me, joint->m_dof_index, M);
			}
		}
		void Assembler::assemble_inertia_elements(void) const
		{
			double* M = m_analysis->m_solver->m_M;
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->inertia(m_Me);
				assemble_matrix(m_Me, element->m_dof_index, M);
			}
		}
		void Assembler::assemble_inertia_constraints(void) const
		{
			//data
			double* M = m_analysis->m_solver->m_M;
			//constraints
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				//data
				const double* cg = constraint->m_gradient_data;
				//assemble
				assemble_vector(cg, constraint->m_dof_index, constraint->m_dof_indexes, M);
				assemble_vector(cg, constraint->m_dof_indexes, constraint->m_dof_index, M);
			}
		}

		void Assembler::assemble_damping_elements(void) const
		{
			double* C = m_analysis->m_solver->m_C;
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->damping(m_Ce);
				assemble_matrix(m_Ce, element->m_dof_index, C);
			}
		}
		void Assembler::assemble_damping_constraints(void) const
		{
			//data
			double* C = m_analysis->m_solver->m_C;
			const double a = boundary::Constraint::m_baumgarte.alpha();
			//constraints
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				//data
				const uint32_t nd = constraint->m_dof.size();
				const math::Vector cg(constraint->m_gradient_data, nd);
				const math::Matrix ch(constraint->m_hessian_data, nd, nd);
				const math::Vector cv(constraint->m_dof_velocity_data, nd);
				//assemble
				const math::Vector cr = ch * cv;
				assemble_vector(cr.data(), constraint->m_dof_index, constraint->m_dof_indexes, C, 2);
				assemble_vector(cg.data(), constraint->m_dof_index, constraint->m_dof_indexes, C, 2 * a);
			}
		}

		void Assembler::assemble_stiffness_joints(void) const
		{
			double* K = m_analysis->m_solver->m_K;
			for(const mesh::joints::Joint* joint : m_analysis->m_model->m_mesh->m_joints)
			{
				joint->stiffness(m_Ke);
				assemble_matrix(m_Ke, joint->m_dof_index, K);
			}
		}
		void Assembler::assemble_stiffness_elements(void) const
		{
			double* K = m_analysis->m_solver->m_K;
			for(const mesh::elements::Element* element : m_analysis->m_model->m_mesh->m_elements)
			{
				element->stiffness(m_Ke);
				assemble_matrix(m_Ke, element->m_dof_index, K);
			}
		}
		void Assembler::assemble_stiffness_constraints(void) const
		{
			//data
			double* K = m_analysis->m_solver->m_K;
			const uint32_t ss = m_analysis->m_solver->state_set();
			const double b = boundary::Constraint::m_baumgarte.beta();
			const double a = boundary::Constraint::m_baumgarte.alpha();
			//constraints
			for(const boundary::Constraint* constraint : m_analysis->m_model->m_boundary->m_constraints)
			{
				//data
				const double cp = constraint->m_state_new;
				const uint32_t nd = constraint->m_dof.size();
				const math::Vector cg(constraint->m_gradient_data, nd);
				const math::Matrix ch(constraint->m_hessian_data, nd, nd);
				const math::Vector cv(constraint->m_dof_velocity_data, nd);
				const math::Vector ca(constraint->m_dof_acceleration_data, nd);
				//assemble
				assemble_matrix(ch.data(), constraint->m_dof_indexes, K, cp);
				assemble_vector(cg.data(), constraint->m_dof_index, constraint->m_dof_indexes, K, b * b);
				if(ss & uint32_t(solvers::state::a))
				{
					math::Vector ct(nd);
					const math::Vector c1 = ch * cv;
					const math::Vector c2 = ch * ca;
					for(uint32_t i = 0; i < nd; i++)
					{
						ct[i] = math::Matrix(constraint->m_third_data + i * nd * nd, nd, nd).bilinear(cv);
					}
					assemble_vector(ct.data(), constraint->m_dof_index, constraint->m_dof_indexes, K);
					assemble_vector(c2.data(), constraint->m_dof_index, constraint->m_dof_indexes, K);
					assemble_vector(c1.data(), constraint->m_dof_index, constraint->m_dof_indexes, K, 2 * a);
				}
				else
				{
					assemble_vector(cg.data(), constraint->m_dof_indexes, constraint->m_dof_index, K);
				}
			}
		}

		//static data
		Analysis* Assembler::m_analysis = nullptr;
	}
}
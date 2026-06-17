//std
#include <cstring>
#include <algorithm>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Joints/Types.hpp"
#include "FEA/inc/Mesh/Joints/Joint.hpp"

#include "FEA/inc/Results/What.hpp"
#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"
#include "FEA/inc/Boundary/Dependencies/Dependency.hpp"

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			//constructors
			Joint::Joint(void) : m_state_data(nullptr), m_energy_data(nullptr)
			{
				return;
			}

			//destructor
			Joint::~Joint(void)
			{
				delete[] m_state_data;
				delete[] m_energy_data;
				for(boundary::Dependency* dependency : m_dependencies)
				{
					delete dependency;
				}
			}

			//serialization
			void Joint::load(FILE* file)
			{
				uint32_t nn;
				if(fscanf(file, "%d", &nn) != 1)
				{
					throw std::runtime_error("Joint loading error!");
				}
				m_nodes.resize(nn);
				for(uint32_t i = 0; i < nn; i++)
				{
					if(fscanf(file, "%d", &m_nodes[i]) != 1)
					{
						throw std::runtime_error("Joint loading error!");
					}
				}
			}
			void Joint::save(FILE* file) const
			{
				fprintf(file, "%02d ", (uint32_t) m_nodes.size());
				for(const uint32_t n : m_nodes)
				{
					fprintf(file, "%04d ", n);
				}
			}

			void Joint::load_state(void)
			{
				//data
				char buffer[800];
				const uint32_t nn = m_nodes.size();
				const uint32_t nt = math::bit_count(state_set());
				const uint32_t ns = m_mesh->model()->results()->steps();
				sprintf(buffer, "%s/Joints/J%04d.txt", m_mesh->model()->folder().c_str(), m_index);
				//check
				if(!m_mesh->model()->results()->what()->joints()) return;
				//allocate
				delete[] m_state_data;
				m_state_data = new double[(ns + 1) * nn * nt];
				//open
				FILE* file = fopen(buffer, "r");
				//load
				if(!file)
				{
					sprintf(buffer, "Unable to open joint %04d results file!", m_index);
					throw std::runtime_error(buffer);
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					for(uint32_t j = 0; j < nn; j++)
					{
						for(uint32_t k = 0; k < nt; k++)
						{
							if(fscanf(file, "%lf", &m_state_data[nn * nt * i + nt * j + k]) != 1)
							{
								fclose(file);
								sprintf(buffer, "Unable to read from joint %04d results file!", m_index);
								throw std::runtime_error(buffer);
							}
						}
					}
				}
				//close
				fclose(file);
			}
			void Joint::save_state(void) const
			{
				//data
				char buffer[800];
				const uint32_t nn = m_nodes.size();
				const uint32_t nt = math::bit_count(state_set());
				const uint32_t ns = m_mesh->model()->results()->steps();
				sprintf(buffer, "%s/Joints/J%04d.txt", m_mesh->model()->folder().c_str(), m_index);
				//check
				if(!m_mesh->model()->results()->what()->joints()) return;
				//open
				FILE* file = fopen(buffer, "w");
				//save
				if(!file)
				{
					sprintf(buffer, "Unable to create joint %04d results file!", m_index);
					throw std::runtime_error(buffer);
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					for(uint32_t j = 0; j < nn; j++)
					{
						for(uint32_t k = 0; k < nt; k++)
						{
							if(fprintf(file, "%+.6e ", m_state_data[nn * nt * i + nt * j + k]) < 0)
							{
								fclose(file);
								sprintf(buffer, "Unable to write to joint %04d reusults file!", m_index);
								throw std::runtime_error(buffer);
							}
						}
					}
					fprintf(file, "\n");
				}
				//close
				fclose(file);
			}

			void Joint::load_energy(void)
			{
				//data
				char buffer[800];
				const uint32_t ns = m_mesh->model()->results()->steps();
				sprintf(buffer, "%s/Energy/J%04d.txt", m_mesh->model()->folder().c_str(), m_index);
				//check
				if(!m_mesh->model()->results()->what()->energies()) return;
				//allocate
				delete[] m_energy_data;
				m_energy_data = new double[2 * (ns + 1)];
				//load
				FILE* file = fopen(buffer, "r");
				if(!file)
				{
					sprintf(buffer, "Unable to create joint %04d energy results file!", m_index);
					throw std::runtime_error(buffer);
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					if(fscanf(file, "%lf %lf", &m_energy_data[2 * i + 0], &m_energy_data[2 * i + 1]) != 2)
					{
						fclose(file);
						sprintf(buffer, "Unable to write to joint %04d energy results file!", m_index);
						throw std::runtime_error(buffer);
					}
				}
				//close
				fclose(file);
			}
			void Joint::save_energy(void) const
			{
				//data
				char buffer[800];
				const uint32_t ns = m_mesh->model()->results()->steps();
				sprintf(buffer, "%s/Energy/J%04d.txt", m_mesh->model()->folder().c_str(), m_index);
				//check
				if(!m_mesh->model()->results()->what()->energies()) return;
				//open
				FILE* file = fopen(buffer, "w");
				//save
				if(!file)
				{
					sprintf(buffer, "Unable to open joint %04d energy results file!", m_index);
					throw std::runtime_error(buffer);
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					if(fprintf(file, "%+.6e %+.6e\n", m_energy_data[2 * i + 0], m_energy_data[2 * i + 1]) < 0)
					{
						fclose(file);
						sprintf(buffer, "Unable to read from joint %04d energy results file!", m_index);
						throw std::runtime_error(buffer);
					}
				}
				//close
				fclose(file);
			}

			//create
			void Joint::create(Joint*& joint, const Joint* base)
			{
				Joint::create(joint, base->type(), base);
			}
			void Joint::create(Joint*& joint, joints::Type type, const Joint* base)
			{
				//data
				delete joint;
				const std::function<void(void)> factory[] = {
					
				};
				// //create
				// for(uint32_t i = 0; 1U << i < uint32_t(Type::last); i++)
				// {
				// 	if(uint32_t(type) == 1U << i)
				// 	{
				// 		factory[i]();
				// 		break;
				// 	}
				// }
			}

			//data
			Mesh* Joint::mesh(void)
			{
				return m_mesh;
			}

			nodes::Node* Joint::node(uint32_t index) const
			{
				return m_mesh->node(m_nodes[index]);
			}
			nodes::Node* Joint::node(uint32_t index, uint32_t node)
			{
				return m_mesh->node(m_nodes[index] = node);
			}

			//types
			uint32_t Joint::nodes_max(void) const
			{
				return 2;
			}
			uint32_t Joint::state_set(void) const
			{
				return 0;
			}
			uint32_t Joint::dof_set(uint32_t) const
			{
				return 0;
			}

			//name
			const char* Joint::type_name(void) const
			{
				return type_name(type());
			}
			const char* Joint::type_name(Type type)
			{
				const char* names[] = {
					"Hinge",
					"Rigid 2D",
					"Rigid 3D",
					"Spherical",
					"Revolute 2D",
					"Revolute 3D"
				};
				return names[math::bit_index(uint32_t(type))];
			}
			const char* Joint::state_name(State state)
			{
				const char* names[] = {
					"Force 1", "Force 2", "Force 3",
					"Moment 1", "Moment 2", "Moment 3"
				};
				return names[math::bit_index(uint32_t(state))];
			}

			//sizes
			const std::vector<uint32_t>& Joint::nodes(void) const
			{
				return m_nodes;
			}

			//index
			uint32_t Joint::index(void) const
			{
				return m_index;
			}
			uint32_t Joint::index_node(uint32_t index) const
			{
				return m_nodes[index];
			}

			//nodes
			void Joint::create_node(uint32_t index)
			{
				m_nodes.push_back(index);
			}
			void Joint::remove_node(uint32_t index)
			{
				m_nodes.erase(m_nodes.begin() + index);
			}

			//results
			double Joint::energy_data(uint32_t type, uint32_t step) const
			{
				return m_energy_data ? m_energy_data[2 * step + type] : 0;
			}
			double Joint::state_data(State state, uint32_t node, uint32_t step) const
			{
				//data
				const uint32_t ss = state_set();
				const uint32_t nn = m_nodes.size();
				const uint8_t ns = math::bit_count(ss);
				const uint8_t sp = math::bit_index(ss, uint32_t(state));
				//return
				return m_state_data && ss & uint32_t(state) ? m_state_data[ns * nn * step + node * ns + sp] : 0;
			}

			//analysis
			void Joint::apply_dof(void) const
			{
				for(uint32_t i = 0; i < m_nodes.size(); i++)
				{
					node(i)->apply_dof(dof_set(i));
				}
			}

			void Joint::setup(void)
			{
				//data
				const uint32_t nn = m_nodes.size();
				const uint32_t nt = math::bit_count(state_set());
				const uint32_t ns = m_mesh->model()->analysis()->solver()->step_max();
				//dofs
				uint8_t p;
				m_dof_index.clear();
				for(uint32_t i = 0; i < m_nodes.size(); i++)
				{
					for(uint32_t j = 1; j < (uint32_t) nodes::dof::last; j <<= 1)
					{
						if(dof_set(i) & j)
						{
							p = math::bit_index(node(i)->m_dof_set, j);
							m_dof_index.push_back(node(i)->m_dof_index[p]);
						}
					}
				}
				//allocate
				delete[] m_state_data;
				delete[] m_energy_data;
				m_energy_data = new double[2 * (ns + 1)];
				m_state_data = new double[(ns + 1) * nn * nt];
			}
			void Joint::record(void)
			{
				return;
			}
			void Joint::update(void)
			{
				return;
			}
			void Joint::restore(void)
			{
				return;
			}
			void Joint::compute(void)
			{
				return;
			}
			bool Joint::check(void) const
			{
				for(uint32_t i : m_nodes)
				{
					if(i >= m_mesh->nodes().size())
					{
						printf("\tJoint %04d has out of range nodes!\n", m_index);
						return false;
					}
				}
				return true;
			}
			void Joint::setup_constraints(void)
			{
				return;
			}
			void Joint::setup_dependencies(void)
			{
				return;
			}

			void Joint::clear_constraints(void)
			{
				for(const boundary::Constraint* constraint : m_constraints)
				{
					delete constraint;
				}
				m_constraints.clear();
			}
			void Joint::clear_dependencies(void)
			{
				for(const boundary::Dependency* dependency : m_dependencies)
				{
					delete dependency;
				}
				m_dependencies.clear();
			}
			boundary::Constraint* Joint::create_constraint(ulist nodes, dlist dof)
			{
				//create
				m_constraints.push_back(new boundary::Constraint(nodes, dof));
				//return
				return m_constraints.back();
			}
			boundary::Dependency* Joint::create_dependency(ulist nodes, dlist dof)
			{
				//create
				m_dependencies.push_back(new boundary::Dependency(nodes[0], dof[0], nodes[1], dof[1]));
				//return
				return m_dependencies.back();
			}

			//energy
			double Joint::kinetic_energy(void) const
			{
				return 0;
			}
			double Joint::internal_energy(void) const
			{
				return 0;
			}

			//forces
			void Joint::inertial_force(double* f) const
			{
				const uint32_t nd = m_dof_index.size();
				memset(f, 0, nd * sizeof(double));
			}
			void Joint::internal_force(double* f) const
			{
				const uint32_t nd = m_dof_index.size();
				memset(f, 0, nd * sizeof(double));
			}

			//tangents
			void Joint::inertia(double* M) const
			{
				const uint32_t nd = m_dof_index.size();
				memset(M, 0, nd * nd * sizeof(double));
			}
			void Joint::damping(double* C) const
			{
				const uint32_t nd = m_dof_index.size();
				memset(C, 0, nd * nd * sizeof(double));
			}
			void Joint::stiffness(double* K) const
			{
				const uint32_t nd = m_dof_index.size();
				memset(K, 0, nd * nd * sizeof(double));
			}

			//static data
			Mesh* Joint::m_mesh = nullptr;
		}
	}
}
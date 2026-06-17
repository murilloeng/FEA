//std
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Linear/Quat.hpp"
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Linear/Mat3.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Results/What.hpp"
#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			//constructors
			Node::Node(void) :
				m_quat_old(nullptr), m_quat_new(nullptr),
				m_state_old(nullptr), m_state_new(nullptr), m_state_data(nullptr), 
				m_velocity_old(nullptr), m_velocity_new(nullptr), m_velocity_data(nullptr), 
				m_acceleration_old(nullptr), m_acceleration_new(nullptr), m_acceleration_data(nullptr),
				m_dof_set(0), m_coordinates{0, 0, 0}
			{
				return;
			}

			//destructor
			Node::~Node(void)
			{
				delete[] m_quat_old;
				delete[] m_quat_new;
				delete[] m_state_old;
				delete[] m_state_new;
				delete[] m_state_data;
				delete[] m_velocity_old;
				delete[] m_velocity_new;
				delete[] m_velocity_data;
				delete[] m_acceleration_new;
				delete[] m_acceleration_old;
				delete[] m_acceleration_data;
			}

			//serialization
			void Node::load(FILE* file)
			{
				if(fscanf(file, "%lf %lf %lf %d", &m_coordinates[0], &m_coordinates[1], &m_coordinates[2], &m_dof_set) != 4)
				{
					throw std::runtime_error("Node loading error!");
				}
			}
			void Node::save(FILE* file) const
			{
				fprintf(file, "%+.6e %+.6e %+.6e %03d", m_coordinates[0], m_coordinates[1], m_coordinates[2], m_dof_set);
			}

			void Node::load_results(void)
			{
				//data
				char buffer[800];
				const bool what[] = {
					m_mesh->model()->results()->what()->states(),
					m_mesh->model()->results()->what()->velocities(),
					m_mesh->model()->results()->what()->accelerations()
				};
				const uint32_t data_set[] = {
					uint32_t(analysis::solvers::state::x),
					uint32_t(analysis::solvers::state::v),
					uint32_t(analysis::solvers::state::a)
				};
				const uint32_t nd = math::bit_count(m_dof_set);
				const std::string folder = m_mesh->model()->folder();
				const uint32_t ns = m_mesh->model()->results()->steps();
				const char* type[] = {"State", "Velocity", "Acceleration"};
				double** data_mem[] = {&m_state_data, &m_velocity_data, &m_acceleration_data};
				const uint32_t solver_set = m_mesh->model()->analysis()->solver()->state_set();
				//load
				for(uint32_t i = 0; i < 3; i++)
				{
					if(solver_set & data_set[i] && what[i])
					{
						//setup
						delete[] *data_mem[i];
						*data_mem[i] = new double[(ns + 1) * nd];
						sprintf(buffer, "%s/%s/N%04d.txt", folder.c_str(), type[i], m_index);
						//load
						FILE* file = fopen(buffer, "r");
						if(!file)
						{
							sprintf(buffer, "Unable to open node %04d %s results file!", m_index, type[i]);
							throw std::runtime_error(buffer);
						}
						for(uint32_t j = 0; j <= ns; j++)
						{
							for(uint32_t k = 0; k < nd; k++)
							{
								if(fscanf(file, "%lf", *data_mem[i] + nd * j + k) != 1)
								{
									sprintf(buffer, "Unable to read from node %04d %s results file!", m_index, type[i]);
									throw std::runtime_error(buffer);
								}
							}
						}
						fclose(file);
					}
				}
			}
			void Node::save_results(void) const
			{
				//data
				char buffer[800];
				const bool what[] = {
					m_mesh->model()->results()->what()->states(),
					m_mesh->model()->results()->what()->velocities(),
					m_mesh->model()->results()->what()->accelerations()
				};
				const uint32_t data_set[] = {
					uint32_t(analysis::solvers::state::x),
					uint32_t(analysis::solvers::state::v),
					uint32_t(analysis::solvers::state::a)
				};
				const uint32_t nd = m_dof_index.size();
				const std::string folder = m_mesh->model()->folder();
				const uint32_t ns = m_mesh->model()->results()->steps();
				const char* type[] = {"State", "Velocity", "Acceleration"};
				const double* data_mem[] = {m_state_data, m_velocity_data, m_acceleration_data};
				const uint32_t solver_set = m_mesh->model()->analysis()->solver()->state_set();
				//save
				for(uint32_t i = 0; i < 3; i++)
				{
					if(solver_set & data_set[i] && what[i])
					{
						//path
						sprintf(buffer, "%s/%s/N%04d.txt", folder.c_str(), type[i], m_index);
						//write
						FILE* file = fopen(buffer, "w");
						if(!file)
						{
							sprintf(buffer, "Unable to create node %04d %s results file!", m_index, type[i]);
							throw std::runtime_error(buffer);
						}
						for(uint32_t j = 0; j <= ns; j++)
						{
							for(uint32_t k = 0; k < nd; k++)
							{
								if(fprintf(file, "%+.6e ", data_mem[i][nd * j + k]) < 0)
								{
									fclose(file);
									sprintf(buffer, "Unable to write to node %04d %s results file!", m_index, type[i]);
									throw std::runtime_error(buffer);
								}
							}
							fprintf(file, "\n");
						}
						fclose(file);
					}
				}
			}

			//data
			Mesh* Node::mesh(void)
			{
				return m_mesh;
			}

			double Node::coordinate(uint32_t index) const
			{
				return m_coordinates[index];
			}
			const double* Node::coordinates(void) const
			{
				return m_coordinates;
			}
			const double* Node::coordinates(const double* coordinates)
			{
				return (const double*) memcpy(m_coordinates, coordinates, 3 * sizeof(double));
			}
			const double* Node::coordinates(double v, uint32_t i)
			{
				m_coordinates[i] = v;
				return m_coordinates;
			}
			const double* Node::coordinates(double x1, double x2, double x3)
			{
				m_coordinates[0] = x1;
				m_coordinates[1] = x2;
				m_coordinates[2] = x3;
				return m_coordinates;
			}

			//index
			uint32_t Node::index(void) const
			{
				return m_index;
			}

			//kinematics
			double* Node::position(double* x) const
			{
				for(uint32_t i = 0; i < 3; i++)
				{
					x[i] = m_coordinates[i];
					if(uint32_t(dof::translation_1) << i & m_dof_set)
					{
						x[i] += m_state_new[math::bit_index(m_dof_set, uint32_t(dof::translation_1) << i)];
					}
				}
				return x;
			}
			const double* Node::quaternion(bool configuration) const
			{
				return configuration ? m_quat_new : m_quat_old;
			}
			const double* Node::rotation(uint32_t type, bool configuration) const
			{
				const double* data[] = {
					m_state_old, m_state_new,
					m_velocity_old, m_velocity_new,
					m_acceleration_old, m_acceleration_new
				};
				return data[2 * type + configuration] + math::bit_index(m_dof_set, uint32_t(dof::rotation_1));
			}
			const double* Node::translation(uint32_t type, bool configuration) const
			{
				const double* data[] = {
					m_state_old, m_state_new,
					m_velocity_old, m_velocity_new,
					m_acceleration_old, m_acceleration_new
				};
				return data[2 * type + configuration] + math::bit_index(m_dof_set, uint32_t(dof::translation_1));
			}

			//transform
			Node& Node::move(const math::Vec3& dx, bool copy)
			{
				if(copy)
				{
					m_mesh->create_node(m_coordinates)->move(dx);
				}
				else
				{
					math::Vec3(m_coordinates + 0) += dx;
				}
				return *this;
			}
			Node& Node::scale(const math::Vec3& xc, double s, bool copy)
			{
				if(copy)
				{
					m_mesh->create_node(m_coordinates)->scale(xc, s);
				}
				else
				{
					const math::Vec3 x0 = m_coordinates;
					math::Vec3(m_coordinates + 0) = xc + s * (x0 - xc);
				}
				return *this;
			}
			Node& Node::rotate(const math::Vec3& xc, const math::Vec3& tr, bool copy)
			{
				if(copy)
				{
					m_mesh->create_node(m_coordinates)->rotate(xc, tr);
				}
				else
				{
					const math::Vec3 x0 = m_coordinates;
					math::Vec3(m_coordinates + 0) = xc + tr.rotate(x0 - xc);
				}
				return *this;
			}
			Node& Node::mirror(const math::Vec3& xc, const math::Vec3& np, bool copy)
			{
				if(copy)
				{
					m_mesh->create_node(m_coordinates)->mirror(xc, np);
				}
				else
				{
					const math::Vec3 xr = math::Vec3(m_coordinates) - xc;
					math::Vec3(m_coordinates + 0) = xc + xr - 2 * np.inner(xr) * np;
				}
				return *this;
			}

			//state
			double& Node::state(dof type)
			{
				return m_state_new[math::bit_index(m_dof_set, uint32_t(type))];
			}
			double& Node::velocity(dof type)
			{
				return m_velocity_new[math::bit_index(m_dof_set, uint32_t(type))];
			}
			double& Node::acceleration(dof type)
			{
				return m_acceleration_new[math::bit_index(m_dof_set, uint32_t(type))];
			}

			const double Node::state(dof type) const
			{
				return m_state_new ? m_state_new[math::bit_index(m_dof_set, uint32_t(type))] : 0;
			}
			const double Node::velocity(dof type) const
			{
				return m_velocity_new ? m_velocity_new[math::bit_index(m_dof_set, uint32_t(type))] : 0;
			}
			const double Node::acceleration(dof type) const
			{
				return m_acceleration_new ? m_acceleration_new[math::bit_index(m_dof_set, uint32_t(type))] : 0;
			}

			//results
			double Node::state_data(dof dof_type, uint32_t step) const
			{
				//data
				const uint8_t nd = math::bit_count(m_dof_set);
				const uint8_t dp = math::bit_index(m_dof_set, uint32_t(dof_type));
				//return
				return m_state_data && m_dof_set & uint32_t(dof_type) ? m_state_data[nd * step + dp] : 0;
			}
			double Node::velocity_data(dof dof_type, uint32_t step) const
			{
				//data
				const uint8_t nd = math::bit_count(m_dof_set);
				const uint8_t dp = math::bit_index(m_dof_set, uint32_t(dof_type));
				//return
				return m_velocity_data && m_dof_set & uint32_t(dof_type) ? m_velocity_data[nd * step + dp] : 0;
			}
			double Node::acceleration_data(dof dof_type, uint32_t step) const
			{
				//data
				const uint8_t nd = math::bit_count(m_dof_set);
				const uint8_t dp = math::bit_index(m_dof_set, uint32_t(dof_type));
				//return
				return m_acceleration_data && m_dof_set & uint32_t(dof_type) ? m_acceleration_data[nd * step + dp] : 0;
			}

			//dof
			const char* Node::dof_name(nodes::dof dof)
			{
				//data
				const char* names[] = {"U₁", "U₂", "U₃", "R₁", "R₂", "R₃", "T"};
				//return
				return names[math::bit_index(uint32_t(dof))];
			}
			const char* Node::load_name(nodes::dof dof)
			{
				//data
				const char* names[] = {"F₁", "F₂", "F₃", "M₁", "M₂", "M₃", "Q"};
				//return
				return names[math::bit_index(uint32_t(dof))];
			}

			uint32_t Node::dof_set(void) const
			{
				return m_dof_set;
			}
			uint32_t Node::dof_size(void) const
			{
				return math::bit_count(m_dof_set);
			}
			uint32_t Node::dof_index(nodes::dof dof) const
			{
				if(~m_dof_set & uint32_t(dof))
				{
					throw std::runtime_error("Node's dof index called with unset dof!");
				}
				return m_dof_index[math::bit_index(m_dof_set, uint32_t(dof))];
			}
			uint32_t Node::dof_index(uint32_t index) const
			{
				return m_dof_index[index];
			}

			//analysis
			void Node::apply_dof(dof dof)
			{
				m_dof_set |= uint32_t(dof);
			}
			void Node::apply_dof(uint32_t dof)
			{
				m_dof_set |= dof;
			}
			void Node::setup_dof(uint32_t& dof_counter)
			{
				//data
				const uint8_t ndof = math::bit_count(m_dof_set);
				//dofs
				m_dof_index.resize(ndof);
				for(uint32_t i = 0; i < ndof; i++)
				{
					m_dof_index[i] = dof_counter + i;
				}
				dof_counter += ndof;
			}

			void Node::check(void)
			{
				return;
			}
			void Node::setup(void)
			{
				setup_memory();
				setup_rotation();
			}

			void Node::record(void)
			{
				//data
				const uint32_t data_set[] = {
					(uint32_t) analysis::solvers::state::x,
					(uint32_t) analysis::solvers::state::v,
					(uint32_t) analysis::solvers::state::a
				};
				const bool what[] = {
					m_mesh->model()->results()->what()->states(),
					m_mesh->model()->results()->what()->velocities(),
					m_mesh->model()->results()->what()->accelerations()
				};
				const uint32_t step = m_mesh->step();
				const uint32_t ndof = m_dof_index.size();
				double* data_new[] = {m_state_new, m_velocity_new, m_acceleration_new};
				double* data_mem[] = {m_state_data, m_velocity_data, m_acceleration_data};
				const uint32_t index = math::bit_index(m_dof_set, uint32_t(dof::rotation_1));
				const uint32_t solver_set = m_mesh->model()->analysis()->solver()->state_set();
				//rotation
				if(m_quat_new)
				{
					math::Vec3(m_state_new + index) = math::Quat(m_quat_new).pseudo(math::Quat(m_quat_old));
				}
				//record
				for(int32_t i = 0; i < 3; i++)
				{
					if(solver_set & data_set[i] && what[i])
					{
						memcpy(data_mem[i] + step * ndof, data_new[i], ndof * sizeof(double));
					}
				}
			}
			void Node::update(void)
			{
				//data
				const uint32_t data_set[] = {
					uint32_t(analysis::solvers::state::x),
					uint32_t(analysis::solvers::state::v),
					uint32_t(analysis::solvers::state::a)
				};
				const uint32_t ndof = (uint32_t) m_dof_index.size();
				double* data_old[] = {m_state_old, m_velocity_old, m_acceleration_old};
				double* data_new[] = {m_state_new, m_velocity_new, m_acceleration_new};
				const uint32_t index = math::bit_index(m_dof_set, uint32_t(dof::rotation_1));
				const uint32_t solver_set = m_mesh->model()->analysis()->solver()->state_set();
				//update
				for(uint32_t i = 0; i < 3; i++)
				{
					if(solver_set & data_set[i])
					{
						memcpy(data_old[i], data_new[i], ndof * sizeof (double));
					}
				}
				//rotation
				if(m_quat_new)
				{
					memset(m_state_new + index, 0, 3 * sizeof(double));
					memset(m_state_old + index, 0, 3 * sizeof(double));
					memcpy(m_quat_old, m_quat_new, 4 * sizeof (double));
				}
			}
			void Node::restore(void)
			{
				//data
				const uint32_t state_set[] = {
					(uint32_t) analysis::solvers::state::x,
					(uint32_t) analysis::solvers::state::v,
					(uint32_t) analysis::solvers::state::a
				};
				const uint32_t ndof = (uint32_t) m_dof_index.size();
				double* data_old[] = {m_state_old, m_velocity_old, m_acceleration_old};
				double* data_new[] = {m_state_new, m_velocity_new, m_acceleration_new};
				const uint32_t index = math::bit_index(m_dof_set, uint32_t(dof::rotation_1));
				const uint32_t solver_set = m_mesh->model()->analysis()->solver()->state_set();
				//restore
				for(uint32_t i = 0; i < 3; i++)
				{
					if(solver_set & state_set[i])
					{
						memcpy(data_new[i], data_old[i], ndof * sizeof (double));
					}
				}
				//rotation
				if(m_quat_new)
				{
					memset(m_state_new + index, 0, 3 * sizeof(double));
					memset(m_state_old + index, 0, 3 * sizeof(double));
					memcpy(m_quat_new, m_quat_old, 4 * sizeof (double));
				}
			}

			void Node::setup_memory(void)
			{
				//data
				const uint32_t data_set[] = {
					(uint32_t) analysis::solvers::state::x,
					(uint32_t) analysis::solvers::state::v,
					(uint32_t) analysis::solvers::state::a
				};
				const char ndof = math::bit_count(m_dof_set);
				const uint32_t steps = m_mesh->model()->analysis()->solver()->step_max();
				const uint32_t state_set = m_mesh->model()->analysis()->solver()->state_set();
				double** data_old[] = {&m_state_old, &m_velocity_old, &m_acceleration_old};
				double** data_new[] = {&m_state_new, &m_velocity_new, &m_acceleration_new};
				double** data_mem[] = {&m_state_data, &m_velocity_data, &m_acceleration_data};
				//setup
				for(uint32_t i = 0; i < 3; i++)
				{
					if(state_set & data_set[i])
					{
						delete[] *data_old[i];
						delete[] *data_new[i];
						delete[] *data_mem[i];
						*data_old[i] = new double[ndof];
						*data_new[i] = new double[ndof];
						*data_mem[i] = new double[(steps + 1) * ndof];
						memset(*data_old[i], 0, ndof * sizeof(double));
						memset(*data_new[i], 0, ndof * sizeof(double));
					}
				}
			}
			void Node::setup_rotation(void)
			{
				const uint32_t dof_rotation =
					(uint32_t) dof::rotation_1 |
					(uint32_t) dof::rotation_2 |
					(uint32_t) dof::rotation_3;
				if((m_dof_set & dof_rotation) == dof_rotation)
				{
					delete[] m_quat_old;
					delete[] m_quat_new;
					m_quat_old = new double[4];
					m_quat_new = new double[4];
					m_quat_old[0] = m_quat_new[0] = 1;
					m_quat_old[1] = m_quat_new[1] = 0;
					m_quat_old[2] = m_quat_new[2] = 0;
					m_quat_old[3] = m_quat_new[3] = 0;
				}
			}

			void Node::update_rotation(void)
			{
				if(m_quat_new)
				{
					const uint32_t r1 = uint32_t(dof::rotation_1);
					math::Quat(m_quat_new + 0) = math::Vec3(m_state_new + math::bit_index(m_dof_set, r1)).quaternion();
				}
			}

			//apply
			void Node::apply_state(void)
			{
				//data
				math::Quat q_new(m_quat_new);
				const math::Quat q_old(m_quat_old);
				const double* x = m_mesh->model()->analysis()->solver()->m_x_new;
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const uint32_t index = math::bit_index(m_dof_set, uint32_t(dof::rotation_1));
				//apply
				for(uint32_t i = 0; i < m_dof_index.size(); i++)
				{
					if(m_dof_index[i] < nu)
					{
						m_state_new[i] = x[m_dof_index[i]];
					}
				}
				//rotation
				if(m_quat_new)
				{
					q_new = math::Vec3(m_state_new + index).quaternion() * q_old;
				}
			}
			void Node::apply_velocity(void)
			{
				//data
				const double* v = m_mesh->model()->analysis()->solver()->m_v_new;
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				//apply
				for(uint32_t i = 0; i < m_dof_index.size(); i++)
				{
					if(m_dof_index[i] < nu)
					{
						m_velocity_new[i] = v[m_dof_index[i]];
					}
				}
			}
			void Node::apply_acceleration(void)
			{
				//data
				const double* a = m_mesh->model()->analysis()->solver()->m_a_new;
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				//apply
				for(uint32_t i = 0; i < m_dof_index.size(); i++)
				{
					if(m_dof_index[i] < nu)
					{
						m_acceleration_new[i] = a[m_dof_index[i]];
					}
				}
			}

			//increment
			void Node::increment_state(void)
			{
				//data
				const double* dx = m_mesh->model()->analysis()->solver()->m_dx;
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				//state
				for(uint32_t i = 0; i < m_dof_index.size(); i++)
				{
					if(m_dof_index[i] < nu)
					{
						m_state_new[i] = m_state_old[i] + dx[m_dof_index[i]];
					}
				}
				//rotation
				if(m_quat_new)
				{
					//index
					const char dp = math::bit_index(m_dof_set, (uint32_t) dof::rotation_1);
					//increment
					const math::Vec3 dt = {
						m_dof_index[dp + 0] < nu ? dx[m_dof_index[dp + 0]] : 0,
						m_dof_index[dp + 1] < nu ? dx[m_dof_index[dp + 1]] : 0,
						m_dof_index[dp + 2] < nu ? dx[m_dof_index[dp + 2]] : 0
					};
					//quaternion
					math::Quat(m_quat_new + 0) = dt.quaternion() * math::Quat(m_quat_old);
				}
			}
			void Node::increment_velocity(void)
			{
				//data
				const double* dv = m_mesh->model()->analysis()->solver()->m_dv;
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				//increment
				for(uint32_t i = 0; i < m_dof_index.size(); i++)
				{
					if(m_dof_index[i] < nu)
					{
						m_velocity_new[i] = m_velocity_old[i] + dv[m_dof_index[i]];
					}
				}
			}
			void Node::increment_acceleration(void)
			{
				//data
				const double* da = m_mesh->model()->analysis()->solver()->m_da;
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				//increment
				for(uint32_t i = 0; i < m_dof_index.size(); i++)
				{
					if(m_dof_index[i] < nu)
					{
						m_acceleration_new[i] = m_acceleration_old[i] + da[m_dof_index[i]];
					}
				}
			}

			//static data
			Mesh* Node::m_mesh = nullptr;
		}
	}
}
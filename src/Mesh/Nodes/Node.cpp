//std
#include <cstring>
#include <stdexcept>

//FEA
#include "FEA/inc/Mesh/Nodes/Node.hpp"

//Math
#include "Math/inc/Miscellaneous/bits.hpp"

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			//constructor
			Node::Node(void) : 
				m_dof{0}, m_index{0},
				m_position_ref{0, 0, 0}, m_position_new{0, 0, 0},
				m_rotation_old{nullptr}, m_rotation_new{nullptr},
				m_state_old{nullptr}, m_state_new{nullptr}, m_state_data{nullptr},
				m_velocity_old{nullptr}, m_velocity_new{nullptr}, m_velocity_data{nullptr},
				m_acceleration_old{nullptr}, m_acceleration_new{nullptr}, m_acceleration_data{nullptr}
			{
				return;
			}

			//destructor
			Node::~Node(void)
			{
				return;
			}

			//save
			void Node::load(FILE* file)
			{
				if(fscanf(file, "%lf %lf %lf %d\n", &m_position_ref[0], &m_position_ref[1], &m_position_ref[2], &m_dof) != 4)
				{
					throw std::runtime_error("Error: Node's loading failed!");
				}
			}
			void Node::save(FILE* file) const
			{
				if(fprintf(file, "%+.6e %+.6e %+.6e %d\n", m_position_ref[0], m_position_ref[1], m_position_ref[2], m_dof) < 0)
				{
					throw std::runtime_error("Error: Node's saving failed!");
				}
			}

			//data
			uint32_t Node::index(void) const
			{
				return m_index;
			}


			const double* Node::rotation(void) const
			{
				return m_rotation_new;
			}

			const double* Node::position(bool configuration)
			{
				if(configuration)
				{
					for(uint32_t i = 0; i < 3; i++)
					{
						const uint32_t dof = uint32_t(DOF::Translation_1) << i;
						m_position_new[i] = m_position_ref[i] + (m_dof & dof) ? m_state_new[math::bit_index(m_dof, dof)] : 0;
					}
				}
				return configuration ? m_position_new : m_position_ref;
			}
			double Node::position(uint32_t index) const
			{
				return m_position_ref[index];
			}
			double Node::position(uint32_t index, double position)
			{
				return m_position_ref[index] = position;
			}

			//DOF
			void Node::apply_DOF(DOF dof)
			{
				m_dof |= uint32_t(dof);
			}
			void Node::apply_DOF(uint32_t dof)
			{
				m_dof |= dof;
			}

			//analysis
			void Node::setup(void)
			{
				// //data
				// const uint32_t rd = 
				// 	uint32_t(DOF::Rotation_1)|
				// 	uint32_t(DOF::Rotation_2)|
				// 	uint32_t(DOF::Rotation_3);
				// const uint32_t nd = math::bit_count(m_dof);
				// //state
				// if(ss & uint32_t(analysis::Data::State)) m_state_old = new double[nd];
				// if(ss & uint32_t(analysis::Data::State)) m_state_new = new double[nd];
				// if(ss & uint32_t(analysis::Data::State)) m_state_data = new double[nd * (ns + 1)];
				// //velocity
				// if(ss & uint32_t(analysis::Data::Velocity)) m_velocity_old = new double[nd];
				// if(ss & uint32_t(analysis::Data::Velocity)) m_velocity_new = new double[nd];
				// if(ss & uint32_t(analysis::Data::Velocity)) m_velocity_data = new double[nd * (ns + 1)];
				// //acceleration
				// if(ss & uint32_t(analysis::Data::Acceleration)) m_acceleration_old = new double[nd];
				// if(ss & uint32_t(analysis::Data::Acceleration)) m_acceleration_new = new double[nd];
				// if(ss & uint32_t(analysis::Data::Acceleration)) m_acceleration_data = new double[nd * (ns + 1)];
				// //allocate rotation
				// if((m_dof & rd) == rd)
				// {
				// 	m_rotation_old = new double[4];
				// 	m_rotation_new = new double[4];
				// 	const double qd[] = {1, 0, 0, 0};
				// 	memcpy(m_rotation_old, qd, 4 * sizeof(double));
				// 	memcpy(m_rotation_new, qd, 4 * sizeof(double));
				// }
			}
			void Node::record(void)
			{
				//data
				const uint32_t step = 0;
				const uint32_t nd = math::bit_count(m_dof);
				double* data_step[] = {m_state_data, m_velocity_data, m_acceleration_data};
				const double* data_new[] = {m_state_new, m_velocity_new, m_acceleration_new};
				//record
				for(uint32_t i = 0; i < 3; i++)
				{
					if(data_step[i]) memcpy(data_step[i] + step * nd, data_new[i], nd * sizeof(double));
				}
			}
			void Node::update(void)
			{
				//data
				const uint32_t nd = math::bit_count(m_dof);
				double* data_old[] = {m_state_old, m_velocity_old, m_acceleration_old};
				const double* data_new[] = {m_state_new, m_velocity_new, m_acceleration_new};
				//update
				for(uint32_t i = 0; i < 3; i++)
				{
					if(data_old[i]) memcpy(data_old[i], data_new[i], nd * sizeof(double));
				}
				if(m_rotation_old) memcpy(m_rotation_old, m_rotation_new, nd * sizeof(double));
			}
			void Node::restore(void)
			{
				//data
				const uint32_t nd = math::bit_count(m_dof);
				double* data_new[] = {m_state_new, m_velocity_new, m_acceleration_new};
				const double* data_old[] = {m_state_old, m_velocity_old, m_acceleration_old};
				//restore
				for(uint32_t i = 0; i < 3; i++)
				{
					if(data_old[i]) memcpy(data_new[i], data_old[i], nd * sizeof(double));
				}
				if(m_rotation_new) memcpy(m_rotation_new, m_rotation_old, nd * sizeof(double));
			}
			void Node::cleanup(void)
			{
				//data
				double** data[] = {
					&m_rotation_old, &m_rotation_new,
					&m_state_old, &m_state_new, &m_state_data,
					&m_velocity_old, &m_velocity_new, &m_velocity_data,
					&m_acceleration_old, &m_acceleration_new, &m_acceleration_data
				};
				//cleanup
				for(double** ptr : data)
				{
					delete[] *ptr;
					*ptr = nullptr;
				}
			}

			//static data
			Mesh* Node::m_mesh = nullptr;
		}
	}
}
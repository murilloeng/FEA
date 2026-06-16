//std
#include <cstring>

//FEA
#include "FEA/inc/Analysis/Data.hpp"
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
				m_dof{0}, m_position{0, 0, 0},
				m_rotation_old{nullptr}, m_rotation_new{nullptr},
				m_state_data{nullptr}, m_velocity_data{nullptr}, m_acceleration_data{nullptr}
			{
				return;
			}
		
			//destructor
			Node::~Node(void)
			{
				return;
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
			void Node::allocate(uint32_t ns, uint32_t ss)
			{
				//data
				const uint32_t rd = 
					uint32_t(DOF::Rotation_1)|
					uint32_t(DOF::Rotation_2)|
					uint32_t(DOF::Rotation_3);
				const uint32_t nd = math::bit_count(m_dof);
				//state
				if(ss & uint32_t(analysis::Data::State)) m_state_old = new double[nd];
				if(ss & uint32_t(analysis::Data::State)) m_state_new = new double[nd];
				if(ss & uint32_t(analysis::Data::State)) m_state_data = new double[nd * (ns + 1)];
				//velocity
				if(ss & uint32_t(analysis::Data::Velocity)) m_velocity_old = new double[nd];
				if(ss & uint32_t(analysis::Data::Velocity)) m_velocity_new = new double[nd];
				if(ss & uint32_t(analysis::Data::Velocity)) m_velocity_data = new double[nd * (ns + 1)];
				//acceleration
				if(ss & uint32_t(analysis::Data::Acceleration)) m_acceleration_old = new double[nd];
				if(ss & uint32_t(analysis::Data::Acceleration)) m_acceleration_new = new double[nd];
				if(ss & uint32_t(analysis::Data::Acceleration)) m_acceleration_data = new double[nd * (ns + 1)];
				//allocate rotation
				if((m_dof & rd) == rd)
				{
					m_rotation_old = new double[4];
					m_rotation_new = new double[4];
					const double qd[] = {1, 0, 0, 0};
					memcpy(m_rotation_old, qd, 4 * sizeof(double));
					memcpy(m_rotation_new, qd, 4 * sizeof(double));
				}
			}
		}
	}
}
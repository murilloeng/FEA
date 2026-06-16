#pragma once

//std
#include <vector>
#include <cstdint>

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node
			{
			public:
				//constructor
				Node(void);
		
				//destructor
				~Node(void);
		
				//DOF
				void apply_DOF(DOF);
				void apply_DOF(uint32_t);
		
			private:
				//analysis
				void cleanup(void);
				void allocate(uint32_t, uint32_t);
		
				//data
				uint32_t m_dof;
				double m_position[3];
				std::vector<uint32_t> m_dof_indexes;
				double *m_rotation_old, *m_rotation_new;
				double *m_state_old, *m_state_new, *m_state_data;
				double *m_velocity_old, *m_velocity_new, *m_velocity_data;
				double *m_acceleration_old, *m_acceleration_new, *m_acceleration_data;
			};
		}
	}
}
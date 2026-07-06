#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		class Mesh;
		namespace nodes
		{
			enum class DOF : uint32_t;
		}
	}
}

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

				//analysis
				void setup(void);
				void compute(void);
				void dof_setup(uint32_t&);

				//data
				uint32_t dof_index(DOF) const;

				double state(DOF) const;
				double velocity(DOF) const;
				double acceleration(DOF) const;

				//data
				uint32_t m_dof_set;
				static Mesh* m_mesh;
				double m_position_ref[3];
				double m_position_new[3];
				double* m_quaternion_old;
				double* m_quaternion_new;
				std::vector<uint32_t> m_dof_indexes;
			};
		}
	}
}
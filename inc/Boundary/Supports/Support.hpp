#pragma once

//std
#include <cstdint>
#include <functional>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node;
			enum class DOF : uint32_t;
		}
	}
	namespace boundary
	{
		class Boundary;
	}
}

namespace fea
{
	namespace boundary
	{
		class Support
		{
		public:
			//constructor
			Support(uint32_t, mesh::nodes::DOF);

			//destructor
			~Support(void);

			//analysis
			void check(void);
			void setup(void);

			//data
			double state(void);
			double velocity(void);
			double acceleration(void);
			mesh::nodes::Node* node(void) const;

			//data
			uint32_t m_node;
			uint32_t m_dof_index;
			mesh::nodes::DOF m_dof;
			static Boundary* m_boundary;
			std::function<double(double)> m_state;
			std::function<double(double)> m_velocity;
			std::function<double(double)> m_acceleration;
		};
	}
}
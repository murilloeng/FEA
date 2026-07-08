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
		private:
			//constructor
			Support(uint32_t, mesh::nodes::DOF);

			//destructor
			~Support(void);

		public:
			//data
			double state(void);
			double velocity(void);
			double acceleration(void);

			uint32_t index_node(void) const;

			mesh::nodes::Node* node(uint32_t);
			mesh::nodes::Node* node(void) const;

			mesh::nodes::DOF dof(void) const;
			mesh::nodes::DOF dof(mesh::nodes::DOF);

		private:
			//analysis
			void check(void);
			void setup(void);

			//data
			uint32_t m_node;
			uint32_t m_dof_index;
			mesh::nodes::DOF m_dof;
			static Boundary* m_boundary;
			std::function<double(double)> m_state;
			std::function<double(double)> m_velocity;
			std::function<double(double)> m_acceleration;

			//friends
			friend class fea::boundary::Boundary;
		};
	}
}
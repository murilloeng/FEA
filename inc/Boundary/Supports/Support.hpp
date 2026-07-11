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

			//types
			typedef std::function<double(double)> Function;

		public:
			//data
			Function state(Function);
			Function state(void) const;

			Function velocity(Function);
			Function velocity(void) const;

			Function acceleration(Function);
			Function acceleration(void) const;

			uint32_t index_node(void) const;

			mesh::nodes::Node* node(uint32_t);
			mesh::nodes::Node* node(void) const;

			mesh::nodes::DOF dof(void) const;
			mesh::nodes::DOF dof(mesh::nodes::DOF);

			//time
			double state(double) const;
			double velocity(double) const;
			double acceleration(double) const;

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
#pragma once

//std
#include <cstdint>

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
		class Initial
		{
		private:
			//constructor
			Initial(void);

			//destructor
			~Initial(void);

			//serialization
			void save(FILE*) const;

		public:
			//data
			double state(double);
			double state(void) const;

			double velocity(double);
			double velocity(void) const;

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
			double m_state;
			double m_velocity;

			uint32_t m_node;
			uint32_t m_index;
			uint32_t m_dof_index;
			mesh::nodes::DOF m_dof;

			static Boundary* m_boundary;

			//friends
			friend class fea::boundary::Boundary;
		};
	}
}
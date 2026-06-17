#pragma once

//std
#include <list>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node;
			enum class dof : uint32_t;
		}
	}
	namespace boundary
	{
		namespace time
		{
			class Time;
		}
		class Boundary;
	}
	namespace analysis
	{
		class Assembler;
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
			Support(void);

			//destructor
			~Support(void);

			//serialization
			void load(FILE*);
			void save(FILE*) const;

			void load_state(void);
			void save_state(void) const;

		public:
			//data
			bool draw_direction(bool);
			bool draw_direction(void) const;

			static Boundary* boundary(void);

			mesh::nodes::Node* node(uint32_t);
			mesh::nodes::Node* node(void) const;

			time::Time* state(uint32_t);
			time::Time* velocity(uint32_t);
			time::Time* acceleration(uint32_t);

			time::Time* state(void) const;
			time::Time* velocity(void) const;
			time::Time* acceleration(void) const;

			mesh::nodes::dof dof(void) const;
			mesh::nodes::dof dof(mesh::nodes::dof);

			//index
			uint32_t index(void) const;
			uint32_t index_node(void) const;
			uint32_t index_state(void) const;
			uint32_t index_velocity(void) const;
			uint32_t index_acceleration(void) const;

			//data
			double dof_state(double) const;
			double dof_velocity(double) const;
			double dof_acceleration(double) const;

			//results
			double state_data(uint32_t) const;

		private:
			//analysis
			void setup(void);
			void record(void);
			void apply(void) const;
			bool check(void) const;
			void apply_dof(void) const;

			//data
			uint32_t m_node;
			uint32_t m_index;
			uint32_t m_dof_index;
			mesh::nodes::dof m_dof;

			uint32_t m_state;
			uint32_t m_velocity;
			uint32_t m_acceleration;

			bool m_draw_direction;
			double* m_reactions_data;

			static Boundary* m_boundary;
			static std::list<uint32_t> m_update_nodes;

			//friends
			friend class boundary::Boundary;
			friend class analysis::Assembler;
		};
	}
}
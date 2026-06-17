#pragma once

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			enum class dof : uint32_t;
		}
	}
	namespace boundary
	{
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
		class Initial
		{
		private:
			//constructors
			Initial(void);

			//destructor
			~Initial(void);

			//serialization
			void load(FILE*);
			void save(FILE*) const;

		public:
			//data
			static Boundary* boundary(void);

			mesh::nodes::dof dof(void) const;
			mesh::nodes::dof dof(mesh::nodes::dof);
			mesh::nodes::Node* node(void) const;
			mesh::nodes::Node* node(uint32_t);

			double state(void) const;
			double state(double);
			double velocity(void) const;
			double velocity(double);

			//index
			uint32_t index(void) const;
			uint32_t index_node(void) const;

		private:
			//analysis
			void setup(void);
			bool check(void) const;
			void apply_dof(void) const;

			//apply
			void apply(void) const;

			//data
			uint32_t m_node;
			uint32_t m_index;
			uint32_t m_dof_index;
			mesh::nodes::dof m_dof;

			double m_state;
			double m_velocity;

			static Boundary* m_boundary;

			//friends
			friend class boundary::Boundary;
			friend class analysis::Assembler;
		};
	}
}
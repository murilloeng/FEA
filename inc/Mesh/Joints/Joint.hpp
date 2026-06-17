#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		class Mesh;
		namespace nodes
		{
			class Node;
			enum class dof : uint32_t;
		}
		namespace joints
		{
			enum class Type : uint32_t;
			enum class State : uint32_t;
		}
	}
	namespace analysis
	{
		class Assembler;
	}
	namespace boundary
	{
		class Boundary;
		class Constraint;
		class Dependency;
	}
}

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			class Joint
			{
			protected:
				//constructors
				Joint(void);

				//destructor
				virtual ~Joint(void);

				//serialization
				virtual void load(FILE*);
				virtual void save(FILE*) const;

				virtual void load_state(void);
				virtual void save_state(void) const;

				virtual void load_energy(void);
				virtual void save_energy(void) const;

				//create
				static void create(Joint*&, const Joint*);
				static void create(Joint*&, joints::Type, const Joint* = nullptr);

				//types
				typedef std::vector<uint32_t> ulist;
				typedef std::vector<mesh::nodes::dof> dlist;

			public:
				//data
				static Mesh* mesh(void);

				nodes::Node* node(uint32_t) const;
				nodes::Node* node(uint32_t, uint32_t);

				//types
				virtual Type type(void) const = 0;
				virtual uint32_t nodes_max(void) const;
				virtual uint32_t state_set(void) const;
				virtual uint32_t dof_set(uint32_t) const;

				//names
				const char* type_name(void) const;
				static const char* type_name(Type);
				static const char* state_name(State);

				//lists
				const std::vector<uint32_t>& nodes(void) const;

				//index
				uint32_t index(void) const;
				uint32_t index_node(uint32_t) const;

				//nodes
				void create_node(uint32_t);
				void remove_node(uint32_t);

				//results
				double energy_data(uint32_t, uint32_t) const;
				double state_data(State, uint32_t, uint32_t) const;

			protected:
				//analysis
				void apply_dof(void) const;

				virtual void setup(void);
				virtual void record(void);
				virtual void update(void);
				virtual void restore(void);
				virtual void compute(void);
				virtual bool check(void) const;
				virtual void setup_constraints(void);
				virtual void setup_dependencies(void);

				void clear_constraints(void);
				void clear_dependencies(void);
				boundary::Constraint* create_constraint(ulist, dlist);
				boundary::Dependency* create_dependency(ulist, dlist);

			public:
				//tangents
				virtual void inertia(double*) const;
				virtual void damping(double*) const;
				virtual void stiffness(double*) const;

				//forces
				virtual void inertial_force(double*) const;
				virtual void internal_force(double*) const;

				//energy
				virtual double kinetic_energy(void) const;
				virtual double internal_energy(void) const;

			protected:
				//data
				uint32_t m_index;
				static Mesh* m_mesh;
				double* m_state_data;
				double* m_energy_data;
				std::vector<uint32_t> m_nodes;
				std::vector<uint32_t> m_dof_index;
				std::vector<boundary::Constraint*> m_constraints;
				std::vector<boundary::Dependency*> m_dependencies;

				//private
				friend class mesh::Mesh;
				friend class boundary::Boundary;
				friend class analysis::Assembler;
			};
		}
	}
}
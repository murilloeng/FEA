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
		}
		namespace elements
		{
			enum class Type : uint32_t;
			enum class State : uint64_t;
		}
	}
	namespace boundary
	{
		namespace loads
		{
			class Element;
		}
	}
	namespace analysis
	{
		class Assembler;
	}
}

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Element
			{
			protected:
				//constructors
				Element(void);

				//destructor
				virtual ~Element(void);

				//serialization
				virtual void load(FILE*);
				virtual void save(FILE*) const;

				virtual void load_state(void);
				virtual void save_state(void) const;

				virtual void load_energy(void);
				virtual void save_energy(void) const;

				//create
				static void create(Element*&, const Element*);
				static void create(Element*&, Type, const Element* = nullptr);

			public:
				//data
				static Mesh* mesh(void);

				nodes::Node* node(uint32_t) const;
				nodes::Node* node(uint32_t, uint32_t);

				//types
				virtual Type type(void) const = 0;
				virtual uint32_t loads_set(void) const = 0;
				virtual uint64_t states_set(void) const = 0;
				virtual uint32_t dof_set(uint32_t) const = 0;

				//name
				const char* type_name(void) const;
				static const char* type_name(Type);

				//state
				static const char* state_name(State);

				//results
				double energy_data(uint32_t, uint32_t) const;
				double state_data(State, uint32_t, uint32_t) const;

				//lists
				const std::vector<uint32_t>& nodes(void) const;

				//index
				uint32_t index(void) const;
				uint32_t index_node(uint32_t) const;

				//nodes
				void create_node(uint32_t);
				void remove_node(uint32_t);

			protected:
				//analysis
				virtual void check(void);
				virtual void setup(void);
				virtual void record(void);
				virtual void update(void);
				virtual void restore(void);
				virtual void compute(void);
				virtual void apply_dof(void) const;

				//data
				void state(double*) const;
				void velocity(double*) const;
				void acceleration(double*) const;

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

				//loads
				virtual void reference_force(double*, const boundary::loads::Element*) const;

			protected:
				//data
				uint32_t m_index;
				static Mesh* m_mesh;
				double* m_state_data;
				double* m_energy_data;
				std::vector<uint32_t> m_nodes;
				std::vector<uint32_t> m_dof_index;

				//friends
				friend class mesh::Mesh;
				friend class analysis::Assembler;
				friend class boundary::loads::Element;
			};
		}
	}
}
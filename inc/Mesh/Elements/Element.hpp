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
				//constructor
				Element(void);

				//destructor
				virtual ~Element(void);

				//serialization
				virtual void save(FILE*) const;

			public:
				//data
				uint32_t index(void) const;
				uint32_t index_node(uint32_t) const;

				void append_node(uint32_t);
				nodes::Node* node(uint32_t) const;
				nodes::Node* node(uint32_t, uint32_t);
				virtual uint32_t dimension(void) const = 0;
				virtual uint32_t dof_set(uint32_t) const = 0;

				//tangents
				virtual void inertia(double*) const = 0;
				virtual void damping(double*) const = 0;
				virtual void stiffness(double*) const = 0;

				//forces
				virtual void internal_force(double*) const = 0;

			protected:
				//analysis
				virtual void check(void);
				virtual void setup(void);
				virtual void update(void);
				virtual void restore(void);
				virtual void compute(void) = 0;

				//data
				uint32_t m_index;
				static Mesh* m_mesh;
				std::vector<uint32_t> m_nodes;
				std::vector<uint32_t> m_dof_indexes;

				//friends
				friend class fea::mesh::Mesh;
				friend class fea::analysis::Assembler;
			};
		}
	}
}
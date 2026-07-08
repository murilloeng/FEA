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
			public:
				//constructor
				Element(void);

				//destructor
				virtual ~Element(void);

				//data
				nodes::Node* node(uint32_t) const;
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
				virtual void compute(void) = 0;

				//data
				static Mesh* m_mesh;
				std::vector<uint32_t> m_nodes;
				std::vector<uint32_t> m_dof_indexes;

				//friends
				friend class mesh::Mesh;
				friend class analysis::Assembler;
			};
		}
	}
}
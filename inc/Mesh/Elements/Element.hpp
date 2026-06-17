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
		}
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

				//serialization
				void load(FILE*);
				void save(FILE*) const;

				//type
				virtual Type type(void) const = 0;

				//dof
				virtual uint32_t dof(uint32_t) const = 0;

				//data
				uint32_t index(void) const;

				nodes::Node* node(uint32_t) const;
				nodes::Node* node(uint32_t, uint32_t);
				const std::vector<uint32_t>& nodes(void) const;

				//compute
				virtual void compute(void) = 0;

				//tangents
				virtual void inertia(double*) const = 0;
				virtual void damping(double*) const = 0;
				virtual void stiffness(double*) const = 0;

				//forces
				virtual void internal_force(double*) const = 0;

			protected:
				//friends
				friend class mesh::Mesh;

				//data
				uint32_t m_index;
				static Mesh* m_mesh;
				std::vector<uint32_t> m_nodes;
			};
		}
	}
}
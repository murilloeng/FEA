#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace draw
	{
		class Data;
		class Mesh;
	}
	namespace mesh
	{
		class Mesh;
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
			public:
				//constructors
				Joint(void);

				//destructor
				virtual ~Joint(void);

				//serialization
				void save(FILE*) const;

			protected:
				//analysis
				virtual void setup(void);
				virtual void create_constraints(void) const = 0;
				virtual void create_dependencies(void) const = 0;

				//draw
				virtual void draw_setup(draw::Data&) const = 0;
				virtual void draw_update(draw::Data&) const = 0;

				//data
				uint32_t m_index;
				static Mesh* m_mesh;
				std::vector<uint32_t> m_nodes;

				//friends
				friend class fea::draw::Mesh;
				friend class fea::mesh::Mesh;
			};
		}
	}
}
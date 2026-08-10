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
				//data
				uint32_t m_index;
				static Mesh* m_mesh;
				std::vector<uint32_t> m_nodes;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}
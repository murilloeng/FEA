#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>

//FEA
#include "FEA/inc/Geometry/Loop.hpp"

namespace fea
{
	namespace geometry
	{
		class Geometry;
	}
	namespace mesh
	{
		namespace elements
		{
			enum class Type : uint32_t;
		}
	}
}

namespace fea
{
	namespace geometry
	{
		class Surface
		{
		protected:
			//constructors
			Surface(void);

			//destructor
			~Surface(void);

			//serialization
			void save(FILE*) const;

		public:
			//data
			bool structured(bool);
			bool structured(void) const;

			static Geometry* geometry(void);

			Loop& loop(uint32_t);
			Loop& create_loop(void);
			const std::vector<Loop>& loops(void) const;

			const std::vector<uint32_t>& nodes(void) const;
			const std::vector<uint32_t>& elements(void) const;

			mesh::elements::Type element_type(void) const;
			mesh::elements::Type element_type(mesh::elements::Type);

			//index
			uint32_t index(void) const;

		protected:
			//mesh
			void mesh(void) const;

			//data
			uint32_t m_index;
			bool m_structured;
			std::vector<Loop> m_loops;
			static Geometry* m_geometry;

			std::vector<uint32_t> m_nodes;
			std::vector<uint32_t> m_elements;

			mesh::elements::Type m_element_type;

			//friends
			friend class fea::geometry::Geometry;
		};
	}
}
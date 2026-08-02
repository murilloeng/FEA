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

			//types
			typedef std::function<void(int32_t, size_t, const size_t*)> function;

		public:
			//data
			bool structured(bool);
			bool structured(void) const;

			static Geometry* geometry(void);

			function generate_elements(function);
			function generate_elements(void) const;
			
			Loop& loop(uint32_t);
			Loop& create_loop(void);
			const std::vector<Loop>& loops(void) const;

			const std::vector<uint32_t>& nodes(void) const;
			const std::vector<uint32_t>& elements(void) const;

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

			function m_generate_elements;

			std::vector<uint32_t> m_nodes;
			std::vector<uint32_t> m_elements;

			//friends
			friend class fea::geometry::Geometry;
		};
	}
}
#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>
#include <functional>

namespace fea
{
	namespace geometry
	{
		class Point;
		class Geometry;
	}
}

namespace fea
{
	namespace geometry
	{
		class Curve
		{
		protected:
			//constructors
			Curve(void);

			//destructor
			virtual ~Curve(void);

			//serialization
			virtual void save(FILE*) const;

			//types
			typedef std::function<void(int32_t, size_t, const size_t*)> function;

		public:
			//data
			static Geometry* geometry(void);

			uint32_t structured(uint32_t);
			uint32_t structured(void) const;

			function generate_elements(function);
			function generate_elements(void) const;

			Point* point(uint32_t) const;
			Point* point(uint32_t, uint32_t);
			const std::vector<uint32_t>& points(void) const;

			const double* major_axis(void) const;
			const double* major_axis(const double*);
			const double* major_axis(double, double, double);

			const std::vector<uint32_t>& nodes(void) const;
			const std::vector<uint32_t>& elements(void) const;

			//index
			uint32_t index(void) const;

		protected:
			//mesh
			virtual void mesh(void) const = 0;

			//data
			uint32_t m_index;
			uint32_t m_structured;
			double m_major_axis[3];
			static Geometry* m_geometry;

			std::vector<uint32_t> m_nodes;
			std::vector<uint32_t> m_points;
			std::vector<uint32_t> m_elements;

			std::function<void(int32_t, size_t, const size_t*)> m_generate_elements;

			//friends
			friend class fea::geometry::Geometry;
		};
	}
}
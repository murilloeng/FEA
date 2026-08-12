#pragma once

//std
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace geometry
	{
		class Curve;
		class Surface;
		class Geometry;
	}
}

namespace fea
{
	namespace geometry
	{
		class Point
		{
		private:
			//constructors
			Point(void);
			Point(const double*, double = 0);
			Point(double, double, double, double = 0);

			//destructor
			~Point(void);

			//serialization
			void save(FILE*) const;

		public:
			//data
			double size(double);
			double size(void) const;

			uint32_t node(void) const;

			static Geometry* geometry(void);

			double position(uint32_t) const;
			const double* position(void) const;
			const double* position(const double*);
			const double* position(uint32_t, double);
			const double* position(double, double, double);

			//index
			uint32_t index(void) const;

		private:
			//analysis
			void check(void) const;

			//mesh
			void mesh(void) const;

			//data
			uint32_t m_node;
			uint32_t m_index;
			static Geometry* m_geometry;
			double m_size, m_position[3];

			//friends
			friend class fea::geometry::Geometry;
		};
	}
}
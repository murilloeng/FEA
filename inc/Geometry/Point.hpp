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

			double coordinate(uint32_t) const;
			const double* coordinates(void) const;
			const double* coordinates(const double*);
			const double* coordinates(double, uint32_t);
			const double* coordinates(double, double, double);

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
			double m_size, m_coordinates[3];

			//friends
			friend class fea::geometry::Geometry;
		};
	}
}
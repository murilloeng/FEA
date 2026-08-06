//std
#include <cstring>
#include <climits>
#include <stdexcept>

//gmsh
#include <gmsh.h>

//FEA
#include "FEA/inc/Geometry/Point.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Point::Point(void) : m_node{UINT_MAX}, m_size{0}, m_coordinates{0, 0, 0}
		{
			return;
		}
		Point::Point(const double* coordinates, double size) : m_node{UINT_MAX}, m_size{size}
		{
			memcpy(m_coordinates, coordinates, 3 * sizeof(double));
		}
		Point::Point(double x, double y, double z, double size) : m_node{UINT_MAX}, m_size{size}, m_coordinates{x, y, z}
		{
			return;
		}

		//destructor
		Point::~Point(void)
		{
			return;
		}

		//serialization
		void Point::save(FILE* file) const
		{
			fprintf(file, "%+.6e %+.6e %+.6e %+.6e %04d", m_coordinates[0], m_coordinates[1], m_coordinates[2], m_size, m_node);
		}

		//data
		double Point::size(void) const
		{
			return m_size;
		}
		double Point::size(double size)
		{
			return m_size = size;
		}

		uint32_t Point::node(void) const
		{
			return m_node;
		}

		Geometry* Point::geometry(void)
		{
			return m_geometry;
		}

		double Point::coordinate(uint32_t index) const
		{
			return m_coordinates[index];
		}
		const double* Point::coordinates(void) const
		{
			return m_coordinates;
		}
		const double* Point::coordinates(double v, uint32_t i)
		{
			m_coordinates[i] = v;
			return m_coordinates;
		}
		const double* Point::coordinates(const double* coordinates)
		{
			return (const double*) memcpy(m_coordinates, coordinates, 3 * sizeof(double));
		}
		const double* Point::coordinates(double x, double y, double z)
		{
			m_coordinates[0] = x;
			m_coordinates[1] = y;
			m_coordinates[2] = z;
			return m_coordinates;
		}


		//index
		uint32_t Point::index(void) const
		{
			return m_index;
		}

		//analysis
		void Point::check(void) const
		{
			if(m_size < 0)
			{
				throw std::runtime_error("Error: Point with negative size!");
			}
		}

		//mesh
		void Point::mesh(void) const
		{
			gmsh::model::geo::addPoint(m_coordinates[0], m_coordinates[1], m_coordinates[2], m_size);
		}

		//static
		Geometry* Point::m_geometry = nullptr;
	}
}
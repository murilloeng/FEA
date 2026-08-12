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
		Point::Point(void) : m_node{UINT_MAX}, m_size{0}, m_position{0, 0, 0}
		{
			return;
		}
		Point::Point(const double* position, double size) : m_node{UINT_MAX}, m_size{size}
		{
			memcpy(m_position, position, 3 * sizeof(double));
		}
		Point::Point(double x1, double x2, double x3, double size) : m_node{UINT_MAX}, m_size{size}, m_position{x1, x2, x3}
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
			fprintf(file, "%+.6e %+.6e %+.6e %+.6e %04d", m_position[0], m_position[1], m_position[2], m_size, m_node);
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

		double Point::position(uint32_t index) const
		{
			return m_position[index];
		}
		const double* Point::position(void) const
		{
			return m_position;
		}
		const double* Point::position(uint32_t i, double x)
		{
			m_position[i] = x;
			return m_position;
		}
		const double* Point::position(const double* position)
		{
			return (const double*) memcpy(m_position, position, 3 * sizeof(double));
		}
		const double* Point::position(double x1, double x2, double x3)
		{
			m_position[0] = x1;
			m_position[1] = x2;
			m_position[2] = x3;
			return m_position;
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
			gmsh::model::geo::addPoint(m_position[0], m_position[1], m_position[2], m_size);
		}

		//static
		Geometry* Point::m_geometry = nullptr;
	}
}
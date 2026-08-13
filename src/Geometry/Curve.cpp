//std
#include <cstring>
#include <stdexcept>

//FEA
#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Mesh/Elements/Type.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Curve::Curve(void) : m_structured{0}, m_major_axis{0, 0, 1}, m_element_type{mesh::elements::Type::Last}
		{
			return;
		}

		//destructor
		Curve::~Curve(void)
		{
			return;
		}

		//serialization
		void Curve::save(FILE* file) const
		{
			const double* x = m_major_axis;
			const uint32_t np = m_points.size();
			fprintf(file, "%d %d ", m_structured, np);
			fprintf(file, "%+.6e %+.6e %+.6e ", x[0], x[1], x[2]);
			for(const uint32_t p : m_points) fprintf(file, "%d ", p);
		}

		//data
		Geometry* Curve::geometry(void)
		{
			return m_geometry;
		}

		uint32_t Curve::structured(void) const
		{
			return m_structured;
		}
		uint32_t Curve::structured(uint32_t structured)
		{
			return m_structured = structured;
		}

		Point* Curve::point(uint32_t index) const
		{
			return m_geometry->point(m_points[index]);
		}
		Point* Curve::point(uint32_t index, uint32_t point)
		{
			return m_geometry->point(m_points[index] = point);
		}
		const std::vector<uint32_t>& Curve::points(void) const
		{
			return m_points;
		}

		const double* Curve::major_axis(void) const
		{
			return m_major_axis;
		}
		const double* Curve::major_axis(const double* major_axis)
		{
			return (const double*) memcpy(m_major_axis, major_axis, 3 * sizeof(double));
		}
		const double* Curve::major_axis(double s1, double s2, double s3)
		{
			m_major_axis[0] = s1;
			m_major_axis[1] = s2;
			m_major_axis[2] = s3;
			return m_major_axis;
		}

		const std::vector<uint32_t>& Curve::nodes(void) const
		{
			return m_nodes;
		}
		const std::vector<uint32_t>& Curve::elements(void) const
		{
			return m_elements;
		}

		mesh::elements::Type Curve::element_type(void) const
		{
			return m_element_type;
		}
		mesh::elements::Type Curve::element_type(mesh::elements::Type element_type)
		{
			return m_element_type = element_type;
		}

		//index
		uint32_t Curve::index(void) const
		{
			return m_index;
		}
		uint32_t Curve::index_point(uint32_t index) const
		{
			return m_points[index];
		}

		//transformations
		Curve* Curve::move(const double* u, bool copy)
		{
			m_geometry->move_curve(m_index, u, copy);
			return copy ? m_geometry->curves().back() : this;
		}
		Curve* Curve::move(double u1, double u2, double u3, bool copy)
		{
			m_geometry->move_curve(m_index, u1, u2, u3, copy);
			return copy ? m_geometry->curves().back() : this;
		}

		Curve* Curve::scale(const double* c, double a, bool copy)
		{
			m_geometry->scale_curve(m_index, c, a, copy);
			return copy ? m_geometry->curves().back() : this;
		}
		Curve* Curve::scale(double c1, double c2, double c3, double a, bool copy)
		{
			m_geometry->scale_curve(m_index, c1, c2, c3, a, copy);
			return copy ? m_geometry->curves().back() : this;
		}

		Curve* Curve::scale(const double* c, const double* a, bool copy)
		{
			m_geometry->scale_curve(m_index, c, a, copy);
			return copy ? m_geometry->curves().back() : this;
		}
		Curve* Curve::scale(double c1, double c2, double c3, double a1, double a2, double a3, bool copy)
		{
			m_geometry->scale_curve(m_index, c1, c2, c3, a1, a2, a3, copy);
			return copy ? m_geometry->curves().back() : this;
		}

		Curve* Curve::rotate(const double* c, const double* r, double t, bool copy)
		{
			m_geometry->rotate_curve(m_index, c, r, t, copy);
			return copy ? m_geometry->curves().back() : this;
		}
		Curve* Curve::rotate(double c1, double c2, double c3, double r1, double r2, double r3, double t, bool copy)
		{
			m_geometry->rotate_curve(m_index, c1, c2, c3, r1, r2, r3, t, copy);
			return copy ? m_geometry->curves().back() : this;
		}

		//analysis
		void Curve::check(void) const
		{
			for(uint32_t point : m_points)
			{
				if(point >= m_geometry->points().size())
				{
					throw std::runtime_error("Error: Curve with out of range point!");
				}
			}
		}

		//draw
		void Curve::draw_setup(draw::Data&) const
		{
			return;
		}
		void Curve::draw_update(draw::Data&) const
		{
			return;
		}

		//static
		Geometry* Curve::m_geometry = nullptr;
	}
}
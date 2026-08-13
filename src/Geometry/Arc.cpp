//gmsh
#include <gmsh.h>

//Canvas
#include "Canvas/inc/Math/vec3.hpp"
#include "Canvas/inc/Vertices/Model3D.hpp"

//FEA
#include "FEA/inc/Draw/Data.hpp"

#include "FEA/inc/Geometry/Arc.hpp"
#include "FEA/inc/Geometry/Point.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Arc::Arc(uint32_t p1, uint32_t p2, uint32_t p3)
		{
			m_points.push_back(p1);
			m_points.push_back(p2);
			m_points.push_back(p3);
		}

		//destructor
		Arc::~Arc(void)
		{
			return;
		}

		//data
		uint32_t Arc::draw_mesh(void)
		{
			return m_draw_mesh;
		}
		uint32_t Arc::draw_mesh(uint32_t draw_mesh)
		{
			return m_draw_mesh = draw_mesh;
		}

		//mesh
		void Arc::mesh(void) const
		{
			gmsh::model::geo::addCircleArc(m_points[0] + 1, m_points[1] + 1, m_points[2] + 1);
		}

		//clone
		Curve* Arc::clone(void) const
		{
			return new Arc(*this);
		}

		//draw
		void Arc::draw_setup(draw::Data& data) const
		{
			data.m_counter_edges += 2 * m_draw_mesh;
			data.m_counter_vertices += m_draw_mesh + 1;
		}
		void Arc::draw_update(draw::Data& data) const
		{
			//data
			const canvas::vec3 x0 = m_geometry->point(m_points[0])->position();
			const canvas::vec3 x1 = m_geometry->point(m_points[1])->position();
			const canvas::vec3 x2 = m_geometry->point(m_points[2])->position();
			uint32_t* ibo_ptr = data.m_ibo.data() + data.m_counter_dots + data.m_index_edges;
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) data.m_vbo.data() + data.m_index_vertices;
			//ibo data
			for(uint32_t i = 0; i < m_draw_mesh; i++)
			{
				ibo_ptr[2 * i + 0] = data.m_index_vertices + i + 0;
				ibo_ptr[2 * i + 1] = data.m_index_vertices + i + 1;
			}
			//vbo data
			const double r = (x0 - x1).norm();
			const canvas::vec3 s1 = (x0 - x1).unit();
			const canvas::vec3 q2 = (x2 - x1).unit();
			const canvas::vec3 s3 = s1.cross(q2).unit();
			const canvas::vec3 s2 = s3.cross(s1).unit();
			const double a = acos(s1.inner(q2));
			for(uint32_t i = 0; i <= m_draw_mesh; i++)
			{
				const float t = float(i) / m_draw_mesh;
				vbo_ptr[i].m_color = data.m_colors.curves();
				vbo_ptr[i].m_position = x1 + r * cos(t * a) * s1 + r * sin(a * t) * s2;
			}
			//update
			data.m_index_edges += 2 * m_draw_mesh;
			data.m_index_vertices += m_draw_mesh + 1;
		}

		//static
		uint32_t Arc::m_draw_mesh = 40;
	}
}
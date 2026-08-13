//gmsh
#include <gmsh.h>

//Canvas
#include "Canvas/inc/Vertices/Model3D.hpp"

//FEA
#include "FEA/inc/Draw/Data.hpp"

#include "FEA/inc/Geometry/Line.hpp"
#include "FEA/inc/Geometry/Point.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Line::Line(uint32_t p1, uint32_t p2)
		{
			m_points.push_back(p1);
			m_points.push_back(p2);
		}

		//destructor
		Line::~Line(void)
		{
			return;
		}

		//mesh
		void Line::mesh(void) const
		{
			gmsh::model::geo::addLine(m_points[0] + 1, m_points[1] + 1);
		}

		//clone
		Curve* Line::clone(void) const
		{
			return new Line(*this);
		}

		//draw
		void Line::draw_setup(draw::Data& data) const
		{
			data.m_counter_edges += 2;
			data.m_counter_vertices += 2;
		}
		void Line::draw_update(draw::Data& data) const
		{
			//data
			uint32_t* ibo_ptr = data.m_ibo.data() + data.m_counter_dots + data.m_index_edges;
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) data.m_vbo.data() + data.m_index_vertices;
			//ibo data
			ibo_ptr[0] = data.m_index_vertices + 0;
			ibo_ptr[1] = data.m_index_vertices + 1;
			//vbo data
			vbo_ptr[0].m_color = vbo_ptr[1].m_color = data.m_colors.curves();
			vbo_ptr[0].m_position = m_geometry->point(m_points[0])->position();
			vbo_ptr[1].m_position = m_geometry->point(m_points[1])->position();
			//updatec
			data.m_index_edges += 2;
			data.m_index_vertices += 2;
		}
	}
}
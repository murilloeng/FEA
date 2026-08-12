//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Draw.hpp"
#include "FEA/inc/Draw/Geometry.hpp"

#include "FEA/inc/Geometry/Point.hpp"
#include "FEA/inc/Geometry/Curve.hpp"
#include "FEA/inc/Geometry/Surface.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

//Canvas
#include "Canvas/inc/Vertices/Model3D.hpp"
#include "Canvas/inc/Cameras/BoundingBox.hpp"

namespace fea
{
	namespace draw
	{
		//constructor
		Geometry::Geometry(const Draw* draw, const geometry::Geometry* geometry) : m_shader{"Model3D"}, m_draw{draw}, m_geometry{geometry}
		{
			//vbo setup
			m_vbo.vertex_size(sizeof(canvas::vertices::Model3D));
			//vao setup
			m_vao.attribute_enable(0);
			m_vao.attribute_enable(1);
			m_vao.attribute_binding(0, 0);
			m_vao.attribute_binding(1, 0);
			m_vao.element_buffer(m_ibo.id());
			m_vao.attribute_format(0, 3, GL_FLOAT, 0 * sizeof(float));
			m_vao.attribute_format(1, 4, GL_FLOAT, 3 * sizeof(float));
			m_vao.vertex_buffer(0, m_vbo.id(), 0, sizeof(canvas::vertices::Model3D));
		}

		//destructor
		Geometry::~Geometry(void)
		{
			return;
		}

		//draw
		void Geometry::draw(void)
		{
			//setup
			m_vao.bind();
			m_shader.bind();
			//draw dots
			uint32_t offset = 0;
			glDrawElements(GL_POINTS, m_counter_dots, GL_UNSIGNED_INT, (void*) uintptr_t(offset));
			//draw edges
			offset += m_counter_dots * sizeof(uint32_t);
			glDrawElements(GL_LINES, m_counter_edges, GL_UNSIGNED_INT, (void*) uintptr_t(offset));
			//draw faces
			offset += m_counter_edges * sizeof(uint32_t);
			glDrawElements(GL_TRIANGLES, m_counter_faces, GL_UNSIGNED_INT, (void*) uintptr_t(offset));
		}
		void Geometry::setup(void)
		{
			//data
			m_counter_dots = 0;
			m_counter_edges = 0;
			m_counter_faces = 0;
			m_counter_vertices = 0;
			//setup
			if(m_draw->what().points()) setup_points();
			if(m_draw->what().curves()) setup_curves();
			if(m_draw->what().surfaces()) setup_surfaces();
			//allocate
			m_vbo.allocate(m_counter_vertices);
			m_ibo.allocate(m_counter_dots + m_counter_edges + m_counter_faces);
		}
		void Geometry::update(void)
		{
			//data
			m_index_dots = 0;
			m_index_edges = 0;
			m_index_faces = 0;
			m_index_vertices = 0;
			//update
			if(m_draw->what().points()) update_points();
			if(m_draw->what().curves()) update_curves();
			if(m_draw->what().surfaces()) update_surfaces();
			//transfers
			m_vbo.transfer();
			m_ibo.transfer();
		}

		//setup
		void Geometry::setup_points(void)
		{
			return;
		}
		void Geometry::setup_curves(void)
		{
			return;
		}
		void Geometry::setup_surfaces(void)
		{
			return;
		}

		//update
		void Geometry::update_points(void)
		{
			return;
		}
		void Geometry::update_curves(void)
		{
			return;
		}
		void Geometry::update_surfaces(void)
		{
			return;
		}
	}
}
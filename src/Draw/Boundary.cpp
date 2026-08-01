//FEA
#include "FEA/inc/Draw/Boundary.hpp"
#include "FEA/inc/Boundary/Boundary.hpp"

//Canvas
#include "Canvas/inc/Vertices/Model3D.hpp"
#include "Canvas/inc/Cameras/BoundingBox.hpp"

namespace fea
{
	namespace draw
	{
		//constructor
		Boundary::Boundary(const Draw* draw, const boundary::Boundary* boundary) : m_shader{"Model3D"}, m_draw{draw}, m_boundary{boundary}
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
		Boundary::~Boundary(void)
		{
			return;
		}

		//draw
		void Boundary::draw(void)
		{
			//setup
			m_vao.bind();
			m_shader.bind();
			uint32_t offset = 0;
			//draw dots
			glDrawElements(GL_POINTS, m_counter_dots, GL_UNSIGNED_INT, (void*) uintptr_t(offset));
			//draw edges
			offset += m_counter_dots * sizeof(uint32_t);
			glDrawElements(GL_LINES, m_counter_edges, GL_UNSIGNED_INT, (void*) uintptr_t(offset));
			//draw faces
			offset += m_counter_edges * sizeof(uint32_t);
			glDrawElements(GL_TRIANGLES, m_counter_faces, GL_UNSIGNED_INT, (void*) uintptr_t(offset));
		}
		void Boundary::setup(void)
		{
			//setup
			m_counter_dots = 0;
			m_counter_edges = 0;
			m_counter_faces = 0;
			m_counter_vertices = 0;
			//allocate
			m_vbo.allocate(m_counter_vertices);
			m_ibo.allocate(m_counter_dots + m_counter_edges + m_counter_faces);
		}
		void Boundary::update(void)
		{
			//setup
			m_index_dots = 0;
			m_index_edges = 0;
			m_index_faces = 0;
			m_index_vertices = 0;
			//transfers
			m_vbo.transfer();
			m_ibo.transfer();
		}

		//setup
		void Boundary::setup_loads(void)
		{
			return;
		}
		void Boundary::setup_supports(void)
		{
			return;
		}

		//update
		void Boundary::update_loads(void)
		{
			return;
		}
		void Boundary::update_supports(void)
		{
			return;
		}
		void Boundary::update_bounding_box(canvas::cameras::BoundingBox& bounding_box) const
		{
			//data
			const uint32_t nv = m_vbo.vertex_count();
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data();
			//update
			for(uint32_t i = 0; i < nv; i++)
			{
				bounding_box.insert_vertex(vbo_ptr[i].m_position.data());
			}
		}
	}
}
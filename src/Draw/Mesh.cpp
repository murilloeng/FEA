//FEA
#include "FEA/inc/Draw/Data.hpp"
#include "FEA/inc/Draw/Draw.hpp"
#include "FEA/inc/Draw/Mesh.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

//Canvas
#include "Canvas/inc/Vertices/Model3D.hpp"
#include "Canvas/inc/Cameras/BoundingBox.hpp"

namespace fea
{
	namespace draw
	{
		//constructor
		Mesh::Mesh(const Draw* draw, const mesh::Mesh* mesh) : m_draw{draw}, m_mesh{mesh}, m_shader{"Model3D"}
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
		Mesh::~Mesh(void)
		{
			return;
		}

		//draw
		void Mesh::draw(void)
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
		void Mesh::setup(void)
		{
			//data
			m_counter_dots = 0;
			m_counter_edges = 0;
			m_counter_faces = 0;
			m_counter_vertices = 0;
			//setup
			if(m_draw->what().nodes()) setup_nodes();
			if(m_draw->what().elements()) setup_elements();
			//allocate
			m_vbo.allocate(m_counter_vertices);
			m_ibo.allocate(m_counter_dots + m_counter_edges + m_counter_faces);
		}
		void Mesh::update(void)
		{
			//data
			m_index_dots = 0;
			m_index_edges = 0;
			m_index_faces = 0;
			m_index_vertices = 0;
			//update
			if(m_draw->what().nodes()) update_nodes();
			if(m_draw->what().elements()) update_elements();
			//transfers
			m_vbo.transfer();
			m_ibo.transfer();
		}

		//setup
		void Mesh::setup_nodes(void)
		{
			m_counter_dots += m_mesh->nodes().size();
			m_counter_vertices += m_mesh->nodes().size();
		}
		void Mesh::setup_elements(void)
		{
			//data
			Data data = {
				m_draw->m_step, m_draw->m_colors,
				m_index_dots, m_index_edges, m_index_faces, m_index_vertices,
				m_counter_dots, m_counter_edges, m_counter_faces, m_counter_vertices,
				m_ibo, m_vbo, m_draw->m_positions_data, m_draw->m_rotations_data
			};
			//setup
			for(const mesh::elements::Element* element : m_mesh->elements())
			{
				element->draw_setup(data);
			}
		}

		//update
		void Mesh::update_nodes(void)
		{
			//data
			const uint32_t nn = m_mesh->nodes().size();
			uint32_t* ibo_ptr = m_ibo.data() + m_index_dots;
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
			//buffers data
			for(uint32_t i = 0; i < nn; i++)
			{
				ibo_ptr[i] = m_index_vertices + i;
				vbo_ptr[i].m_color = m_draw->m_colors.nodes();
				vbo_ptr[i].m_position = m_draw->m_positions_data + 3 * nn * m_draw->m_step + 3 * i;
			}
			//update
			m_index_dots += nn;
			m_index_vertices += nn;
		}
		void Mesh::update_elements(void)
		{
			//data
			Data data = {
				m_draw->m_step, m_draw->m_colors,
				m_index_dots, m_index_edges, m_index_faces, m_index_vertices,
				m_counter_dots, m_counter_edges, m_counter_faces, m_counter_vertices,
				m_ibo, m_vbo, m_draw->m_positions_data, m_draw->m_rotations_data
			};
			//update
			for(const mesh::elements::Element* element : m_mesh->elements())
			{
				element->draw_update(data);
			}
		}
	}
}
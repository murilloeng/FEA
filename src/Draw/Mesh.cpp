//FEA
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
			setup_nodes();
			for(const mesh::elements::Element* element : m_mesh->elements())
			{
				if(element->dimension() == 1) setup_element_1D(element);
				if(element->dimension() == 2) setup_element_2D(element);
				if(element->dimension() == 3) setup_element_3D(element);
			}
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
			update_nodes();
			for(const mesh::elements::Element* element : m_mesh->elements())
			{
				if(element->dimension() == 1) update_element_1D(element);
				if(element->dimension() == 2) update_element_2D(element);
				if(element->dimension() == 3) update_element_3D(element);
			}
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
		void Mesh::setup_element_1D(const mesh::elements::Element* element)
		{
			m_counter_edges += 2;
			m_counter_vertices += 2;
		}
		void Mesh::setup_element_2D(const mesh::elements::Element* element)
		{
			return;
		}
		void Mesh::setup_element_3D(const mesh::elements::Element* element)
		{
			return;
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
				vbo_ptr[i].m_color = m_draw->colors().nodes();
				vbo_ptr[i].m_position = m_mesh->node(i)->position_ref();
			}
			//update
			m_index_dots += nn;
			m_index_vertices += nn;
		}
		void Mesh::update_element_1D(const mesh::elements::Element* element)
		{
			//data
			uint32_t* ibo_ptr = m_ibo.data() + m_counter_dots + m_index_edges;
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
			//ibo data
			ibo_ptr[0] = m_index_vertices + 0;
			ibo_ptr[1] = m_index_vertices + 1;
			//vbo data
			vbo_ptr[0].m_color = m_draw->colors().elements();
			vbo_ptr[1].m_color = m_draw->colors().elements();
			vbo_ptr[0].m_position = element->node(0)->position_ref();
			vbo_ptr[1].m_position = element->node(1)->position_ref();
			//update
			m_index_edges += 2;
			m_index_vertices += 2;
		}
		void Mesh::update_element_2D(const mesh::elements::Element* element)
		{
			return;
		}
		void Mesh::update_element_3D(const mesh::elements::Element* element)
		{
			return;
		}
		void Mesh::update_bounding_box(canvas::cameras::BoundingBox& bounding_box) const
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
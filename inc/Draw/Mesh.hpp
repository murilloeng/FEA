#pragma once

//Canvas
#include "Canvas/inc/Buffers/IBO.hpp"
#include "Canvas/inc/Buffers/VAO.hpp"
#include "Canvas/inc/Buffers/VBO.hpp"
#include "Canvas/inc/Shaders/Shader.hpp"
#include "Canvas/inc/Objects/Object.hpp"

namespace fea
{
	namespace draw
	{
		class Draw;
	}
	namespace mesh
	{
		class Mesh;
		namespace elements
		{
			class Element;
		}
	}
}

namespace fea
{
	namespace draw
	{
		class Mesh : public canvas::objects::Object
		{
		private:
			//constructor
			Mesh(const Draw*, const mesh::Mesh*);

			//destructor
			~Mesh(void);

			//draw
			void draw(void) override;
			void setup(void) override;
			void update(void) override;

			//setup
			void setup_nodes(void);
			void setup_element_1D(const mesh::elements::Element*);
			void setup_element_2D(const mesh::elements::Element*);
			void setup_element_3D(const mesh::elements::Element*);

			//update
			void update_nodes(void);
			void update_element_1D(const mesh::elements::Element*);
			void update_element_2D(const mesh::elements::Element*);
			void update_element_3D(const mesh::elements::Element*);

			//data
			const Draw* m_draw;
			const mesh::Mesh* m_mesh;

			uint32_t m_index_dots;
			uint32_t m_index_edges;
			uint32_t m_index_faces;
			uint32_t m_index_vertices;

			uint32_t m_counter_dots;
			uint32_t m_counter_edges;
			uint32_t m_counter_faces;
			uint32_t m_counter_vertices;

			canvas::buffers::IBO m_ibo;
			canvas::buffers::VAO m_vao;
			canvas::buffers::VBO m_vbo;
			canvas::shaders::Shader m_shader;

			//friends
			friend class fea::draw::Draw;
		};
	}
}
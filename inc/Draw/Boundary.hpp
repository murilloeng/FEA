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
	namespace boundary
	{
		class Boundary;
	}
}

namespace fea
{
	namespace draw
	{
		class Boundary : public canvas::objects::Object
		{
		private:
			//constructor
			Boundary(const Draw*, const boundary::Boundary*);

			//destructor
			~Boundary(void);

			//draw
			void draw(void) override;
			void setup(void) override;
			void update(void) override;

			//setup
			void setup_loads(void);
			void setup_supports(void);

			//update
			void update_loads(void);
			void update_supports(void);
			void update_bounding_box(canvas::cameras::BoundingBox&) const override;

		private:
			//data
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

			const Draw* m_draw;
			const boundary::Boundary* m_boundary;

			//friends
			friend class fea::draw::Draw;
		};
	}
}
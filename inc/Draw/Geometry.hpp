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
	namespace geometry
	{
		class Geometry;
	}
}

namespace fea
{
	namespace draw
	{
		class Geometry : public canvas::objects::Object
		{
		private:
			//constructor
			Geometry(const Draw*, const geometry::Geometry*);

			//destructor
			~Geometry(void);

		private:
			//draw
			void draw(void) override;
			void setup(void) override;
			void update(void) override;

			//setup
			void setup_points(void);
			void setup_curves(void);
			void setup_surfaces(void);

			//update
			void update_points(void);
			void update_curves(void);
			void update_surfaces(void);

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
			const geometry::Geometry* m_geometry;

			//friends
			friend class fea::draw::Draw;
		};
	}
}
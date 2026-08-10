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
		namespace nodes
		{
			enum class DOF : uint32_t;
		}
	}
	namespace boundary
	{
		class Support;
		class Boundary;
		namespace loads
		{
			class Node;
		}
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

			//data
			uint32_t load_case(uint32_t);
			uint32_t load_case(void) const;

		private:
			//draw
			void draw(void) override;
			void setup(void) override;
			void update(void) override;

			//setup
			void setup_loads(void);
			void setup_supports(void);

			void setup_load_force(void);
			void setup_load_moment(void);

			void setup_support_position(void);
			void setup_support_rotation(void);

			//update
			void update_loads(void);
			void update_supports(void);

			void update_load_force(const boundary::loads::Node*);
			void update_load_moment(const boundary::loads::Node*);

			void update_support_position(const boundary::Support*);
			void update_support_rotation(const boundary::Support*);

			//transform
			void transform(const boundary::Support*, uint32_t) const;
			void transform(const boundary::loads::Node*, uint32_t) const;

			//data
			uint32_t m_load_case;

			uint32_t m_index_edges;
			uint32_t m_index_faces;
			uint32_t m_index_vertices;

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
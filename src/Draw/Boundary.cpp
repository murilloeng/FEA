//std
#include <cmath>

//Math
#include "Math/inc/Miscellaneous/util.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Draw.hpp"
#include "FEA/inc/Draw/Boundary.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

//Canvas
#include "Canvas/inc/Vertices/Model3D.hpp"
#include "Canvas/inc/Cameras/BoundingBox.hpp"

namespace fea
{
	namespace draw
	{
		//constructor
		Boundary::Boundary(const Draw* draw, const boundary::Boundary* boundary) : 
			m_load_case{0}, m_shader{"Model3D"}, m_draw{draw}, m_boundary{boundary}
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

		//data
		uint32_t Boundary::load_case(void) const
		{
			return m_load_case;
		}
		uint32_t Boundary::load_case(uint32_t load_case)
		{
			return m_load_case = load_case;
		}

		//draw
		void Boundary::draw(void)
		{
			//setup
			m_vao.bind();
			m_shader.bind();
			uint32_t offset = 0;
			//draw edges
			glDrawElements(GL_LINES, m_counter_edges, GL_UNSIGNED_INT, (void*) uintptr_t(offset));
			//draw faces
			offset += m_counter_edges * sizeof(uint32_t);
			glDrawElements(GL_TRIANGLES, m_counter_faces, GL_UNSIGNED_INT, (void*) uintptr_t(offset));
		}
		void Boundary::setup(void)
		{
			//data
			m_counter_edges = 0;
			m_counter_faces = 0;
			m_counter_vertices = 0;
			//setup
			if(m_draw->what().loads()) setup_loads();
			if(m_draw->what().supports()) setup_supports();
			//allocate
			m_vbo.allocate(m_counter_vertices);
			m_ibo.allocate(m_counter_edges + m_counter_faces);
		}
		void Boundary::update(void)
		{
			//data
			m_index_edges = 0;
			m_index_faces = 0;
			m_index_vertices = 0;
			//update
			if(m_draw->what().loads()) update_loads();
			if(m_draw->what().supports()) update_supports();
			//transfers
			m_vbo.transfer();
			m_ibo.transfer();
		}

		//setup
		void Boundary::setup_loads(void)
		{
			//data
			const mesh::nodes::DOF dr[] = {
				mesh::nodes::DOF::Rotation_1, mesh::nodes::DOF::Rotation_2, mesh::nodes::DOF::Rotation_3
			};
			const mesh::nodes::DOF dp[] = {
				mesh::nodes::DOF::Translation_1, mesh::nodes::DOF::Translation_2, mesh::nodes::DOF::Translation_3
			};
			//setup
			for(const boundary::loads::Node* load : m_boundary->load_case(m_load_case)->loads_nodes())
			{
				if(std::find(dp, dp + 3, load->dof()) != dp + 3) setup_load_force();
				if(std::find(dr, dr + 3, load->dof()) != dr + 3) setup_load_moment();
			}
		}
		void Boundary::setup_supports(void)
		{
			//data
			const mesh::nodes::DOF dr[] = {
				mesh::nodes::DOF::Rotation_1, mesh::nodes::DOF::Rotation_2, mesh::nodes::DOF::Rotation_3
			};
			const mesh::nodes::DOF dp[] = {
				mesh::nodes::DOF::Translation_1, mesh::nodes::DOF::Translation_2, mesh::nodes::DOF::Translation_3
			};
			//setup
			for(const boundary::Support* support : m_boundary->supports())
			{
				if(std::find(dp, dp + 3, support->dof()) != dp + 3) setup_support_position();
				if(std::find(dr, dr + 3, support->dof()) != dr + 3) setup_support_rotation();
			}
		}

		void Boundary::setup_load_force(void)
		{
			m_counter_edges += 10;
			m_counter_vertices += 6;
		}
		void Boundary::setup_load_moment(void)
		{
			m_counter_edges += 18;
			m_counter_vertices += 11;
		}

		void Boundary::setup_support_position(void)
		{
			m_counter_edges += 16;
			m_counter_faces += 18;
			m_counter_vertices += 10;
		}
		void Boundary::setup_support_rotation(void)
		{
			m_counter_edges += 30;
			m_counter_faces += 36;
			m_counter_vertices += 20;
		}

		//update
		void Boundary::update_loads(void)
		{
			//data
			const mesh::nodes::DOF dr[] = {
				mesh::nodes::DOF::Rotation_1, mesh::nodes::DOF::Rotation_2, mesh::nodes::DOF::Rotation_3
			};
			const mesh::nodes::DOF dp[] = {
				mesh::nodes::DOF::Translation_1, mesh::nodes::DOF::Translation_2, mesh::nodes::DOF::Translation_3
			};
			//update
			for(const boundary::loads::Node* load : m_boundary->load_case(0)->loads_nodes())
			{
				if(std::find(dp, dp + 3, load->dof()) != dp + 3) update_load_force(load);
				if(std::find(dr, dr + 3, load->dof()) != dr + 3) update_load_moment(load);
			}
		}
		void Boundary::update_supports(void)
		{
			//data
			const mesh::nodes::DOF dr[] = {
				mesh::nodes::DOF::Rotation_1, mesh::nodes::DOF::Rotation_2, mesh::nodes::DOF::Rotation_3
			};
			const mesh::nodes::DOF dp[] = {
				mesh::nodes::DOF::Translation_1, mesh::nodes::DOF::Translation_2, mesh::nodes::DOF::Translation_3
			};
			//update
			for(const boundary::Support* support : m_boundary->supports())
			{
				if(std::find(dp, dp + 3, support->dof()) != dp + 3) update_support_position(support);
				if(std::find(dr, dr + 3, support->dof()) != dr + 3) update_support_rotation(support);
			}
		}

		void Boundary::update_load_force(const boundary::loads::Node* load)
		{
			//data
			const float t = float(M_PI) / 6;
			const float s = math::sign(load->value());
			const canvas::Color color = m_draw->colors().loads();
			const uint32_t ibo_data[] = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5};
			//buffers data
			uint32_t* ibo_ptr = m_ibo.data() + m_index_edges;
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
			//vbo position data
			vbo_ptr[0].m_position = {0, 0, 0};
			vbo_ptr[1].m_position = {-s, 0, 0};
			vbo_ptr[2].m_position = {-0.3f * s * cosf(t), -0.3f * s * sinf(t), 0};
			vbo_ptr[3].m_position = {-0.3f * s * cosf(t), +0.3f * s * sinf(t), 0};
			vbo_ptr[4].m_position = {-0.3f * s * cosf(t), 0, -0.3f * s * sinf(t)};
			vbo_ptr[5].m_position = {-0.3f * s * cosf(t), 0, +0.3f * s * sinf(t)};
			//vbo color data
			for(uint32_t i = 0; i < 6; i++) vbo_ptr[i].m_color = color;
			//ibo data
			for(uint32_t i = 0; i < 10; i++) ibo_ptr[i] = ibo_data[i] + m_index_vertices;
			//transform
			transform(load, 6);
			//update
			m_index_edges += 10;
			m_index_vertices += 6;
		}
		void Boundary::update_load_moment(const boundary::loads::Node* load)
		{
			//data
			const float t = float(M_PI) / 6;
			const float s = math::sign(load->value());
			const canvas::Color color = m_draw->colors().loads();
			const uint32_t ibo_data[] = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 6, 7, 6, 8, 6, 9, 6, 10};
			//buffers data
			uint32_t* ibo_ptr = m_ibo.data() + m_index_edges;
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
			//vbo position data
			vbo_ptr[ 0].m_position = {0, 0, 0};
			vbo_ptr[ 1].m_position = {-s, 0, 0};
			vbo_ptr[ 6].m_position = {-s / 2, 0, 0};
			vbo_ptr[ 2].m_position = {-0.3f * s * cosf(t), -0.3f * s * sinf(t), 0};
			vbo_ptr[ 3].m_position = {-0.3f * s * cosf(t), +0.3f * s * sinf(t), 0};
			vbo_ptr[ 4].m_position = {-0.3f * s * cosf(t), 0, -0.3f * s * sinf(t)};
			vbo_ptr[ 5].m_position = {-0.3f * s * cosf(t), 0, +0.3f * s * sinf(t)};
			vbo_ptr[ 7].m_position = {-s / 2 - 0.3f * s * cosf(t), -0.3f * s * sinf(t), 0};
			vbo_ptr[ 8].m_position = {-s / 2 - 0.3f * s * cosf(t), +0.3f * s * sinf(t), 0};
			vbo_ptr[ 9].m_position = {-s / 2 - 0.3f * s * cosf(t), 0, -0.3f * s * sinf(t)};
			vbo_ptr[10].m_position = {-s / 2 - 0.3f * s * cosf(t), 0, +0.3f * s * sinf(t)};
			//vbo color data
			for(uint32_t i = 0; i < 11; i++) vbo_ptr[i].m_color = color;
			//ibo data
			for(uint32_t i = 0; i < 18; i++) ibo_ptr[i] = ibo_data[i] + m_index_vertices;
			//transform
			transform(load, 11);
			//update
			m_index_edges += 18;
			m_index_vertices += 11;
		}

		void Boundary::update_support_position(const boundary::Support* support)
		{
			//data
			const float t = float(M_PI) / 6;
			const canvas::Color color_faces = m_draw->colors().supports();
			const canvas::Color color_edges = m_draw->colors().background().inverse();
			const uint32_t ibo_data_edges[] = {0, 1, 0, 2, 0, 3, 0, 4, 1, 2, 2, 3, 3, 4, 4, 1};
			const uint32_t ibo_data_faces[] = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1, 1, 2, 3, 1, 3, 4};
			//buffers data
			uint32_t* ibo_ptr_edges = m_ibo.data() + m_index_edges;
			uint32_t* ibo_ptr_faces = m_ibo.data() + m_counter_edges + m_index_faces;
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
			//vbo position data
			vbo_ptr[0 + 5 * 0].m_position = vbo_ptr[0 + 5 * 1].m_position = {0, 0, 0};
			vbo_ptr[1 + 5 * 0].m_position = vbo_ptr[1 + 5 * 1].m_position = {-cosf(t), -sinf(t), -sinf(t)};
			vbo_ptr[2 + 5 * 0].m_position = vbo_ptr[2 + 5 * 1].m_position = {-cosf(t), +sinf(t), -sinf(t)};
			vbo_ptr[3 + 5 * 0].m_position = vbo_ptr[3 + 5 * 1].m_position = {-cosf(t), +sinf(t), +sinf(t)};
			vbo_ptr[4 + 5 * 0].m_position = vbo_ptr[4 + 5 * 1].m_position = {-cosf(t), -sinf(t), +sinf(t)};
			//vbo color data
			for(uint32_t i = 0; i < 5; i++) vbo_ptr[i + 5 * 0].m_color = color_edges;
			for(uint32_t i = 0; i < 5; i++) vbo_ptr[i + 5 * 1].m_color = color_faces;
			//ibo data
			for(uint32_t i = 0; i < 16; i++) ibo_ptr_edges[i] = ibo_data_edges[i] + m_index_vertices;
			for(uint32_t i = 0; i < 18; i++) ibo_ptr_faces[i] = ibo_data_faces[i] + m_index_vertices + 5;
			//transform
			transform(support, 10);
			//update
			m_index_edges += 16;
			m_index_faces += 18;
			m_index_vertices += 10;
		}
		void Boundary::update_support_rotation(const boundary::Support* support)
		{
			//data
			const float t = float(M_PI) / 6;
			const uint32_t ibo_data_edges[] = {
				0, 1, 2, 4, 3, 5, 2, 3, 3, 4, 
				4, 5, 5, 2, 6, 7, 7, 8, 8, 9, 
				9, 6, 2, 6, 3, 7, 4, 8, 5, 9
			};
			const uint32_t ibo_data_faces[] = {
				2, 3, 4, 2, 4, 5, 6, 7, 8, 6, 8, 9,
				2, 3, 7, 2, 7, 6, 3, 4, 8, 3, 8, 7,
				4, 5, 9, 4, 9, 8, 5, 2, 6, 5, 6, 9
			};
			const canvas::Color color_faces = m_draw->colors().supports();
			const canvas::Color color_edges = m_draw->colors().background().inverse();
			//buffers data
			uint32_t* ibo_ptr_edges = m_ibo.data() + m_index_edges;
			uint32_t* ibo_ptr_faces = m_ibo.data() + m_counter_edges+ m_index_faces;
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
			//vbo positon data
			vbo_ptr[0 + 10 * 0].m_position = vbo_ptr[0 + 10 * 1].m_position = {0, 0, 0};
			vbo_ptr[1 + 10 * 0].m_position = vbo_ptr[1 + 10 * 1].m_position = {-cosf(t), 0, 0};
			vbo_ptr[6 + 10 * 0].m_position = vbo_ptr[6 + 10 * 1].m_position = {-1, -cosf(t), -cosf(t)};
			vbo_ptr[7 + 10 * 0].m_position = vbo_ptr[7 + 10 * 1].m_position = {-1, +cosf(t), -cosf(t)};
			vbo_ptr[8 + 10 * 0].m_position = vbo_ptr[8 + 10 * 1].m_position = {-1, +cosf(t), +cosf(t)};
			vbo_ptr[9 + 10 * 0].m_position = vbo_ptr[9 + 10 * 1].m_position = {-1, -cosf(t), +cosf(t)};
			vbo_ptr[2 + 10 * 0].m_position = vbo_ptr[2 + 10 * 1].m_position = {-cosf(t), -cosf(t), -cosf(t)};
			vbo_ptr[3 + 10 * 0].m_position = vbo_ptr[3 + 10 * 1].m_position = {-cosf(t), +cosf(t), -cosf(t)};
			vbo_ptr[4 + 10 * 0].m_position = vbo_ptr[4 + 10 * 1].m_position = {-cosf(t), +cosf(t), +cosf(t)};
			vbo_ptr[5 + 10 * 0].m_position = vbo_ptr[5 + 10 * 1].m_position = {-cosf(t), -cosf(t), +cosf(t)};
			//vbo color data
			for(uint32_t i = 0; i < 10; i++) vbo_ptr[i + 10 * 0].m_color = color_edges;
			for(uint32_t i = 0; i < 10; i++) vbo_ptr[i + 10 * 1].m_color = color_faces;
			//ibo data
			for(uint32_t i = 0; i < 30; i++) ibo_ptr_edges[i] = ibo_data_edges[i] + m_index_vertices;
			for(uint32_t i = 0; i < 36; i++) ibo_ptr_faces[i] = ibo_data_faces[i] + m_index_vertices + 10;
			//transform
			transform(support, 20);
			//update
			m_index_edges += 30;
			m_index_faces += 36;
			m_index_vertices += 20;
		}

		//transform
		void Boundary::transform(const boundary::Support* support, uint32_t count) const
		{
			//data
			const uint32_t d2 = 
				1 << uint32_t(mesh::nodes::DOF::Rotation_2) |
				1 << uint32_t(mesh::nodes::DOF::Translation_2);
			const uint32_t d3 = 
				1 << uint32_t(mesh::nodes::DOF::Rotation_3) |
				1 << uint32_t(mesh::nodes::DOF::Translation_3);
			const float* x = m_draw->position(support->index_node());
			const float s = m_draw->sizes().supports() * m_draw->m_bounding_box.radius();
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
			//transformation
			canvas::mat4 A = canvas::mat4::shifting(x) * canvas::mat4::scaling(s);
			if(1 << uint32_t(support->dof()) & d2) A = A * canvas::mat4::rotation({0, 0, +M_PI_2});
			if(1 << uint32_t(support->dof()) & d3) A = A * canvas::mat4::rotation({0, -M_PI_2, 0});
			//update
			for(uint32_t i = 0; i < count; i++) vbo_ptr[i].m_position *= A;
		}
		void Boundary::transform(const boundary::loads::Node* load, uint32_t count) const
		{
			//data
			const uint32_t d2 = 
				1 << uint32_t(mesh::nodes::DOF::Rotation_2) |
				1 << uint32_t(mesh::nodes::DOF::Translation_2);
			const uint32_t d3 = 
				1 << uint32_t(mesh::nodes::DOF::Rotation_3) |
				1 << uint32_t(mesh::nodes::DOF::Translation_3);
			const float* x = m_draw->position(load->index_node());
			const float s = m_draw->sizes().loads() * m_draw->m_bounding_box.radius();
			canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
			//transformation
			canvas::mat4 A = canvas::mat4::shifting(x) * canvas::mat4::scaling(s);
			if(1 << uint32_t(load->dof()) & d2) A = A * canvas::mat4::rotation({0, 0, +M_PI_2});
			if(1 << uint32_t(load->dof()) & d3) A = A * canvas::mat4::rotation({0, -M_PI_2, 0});
			//update
			for(uint32_t i = 0; i < count; i++) vbo_ptr[i].m_position *= A;
		}
	}
}
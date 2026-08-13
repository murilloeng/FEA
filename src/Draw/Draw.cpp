//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"

#include "FEA/inc/Draw/Draw.hpp"
#include "FEA/inc/Draw/Mesh.hpp"
#include "FEA/inc/Draw/Boundary.hpp"
#include "FEA/inc/Draw/Geometry.hpp"

#include "FEA/inc/Geometry/Point.hpp"
#include "FEA/inc/Geometry/Geometry.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace draw
	{
		//constructor
		Draw::Draw(const Model* model) : 
			m_mesh{new Mesh{this, model->mesh()}}, 
			m_boundary{new Boundary{this, model->boundary()}}, 
			m_geometry{new Geometry{this, model->geometry()}},
			m_model{model}, m_scale{1.0f}, m_step{0}
		{
			//data
			const uint32_t nn = model->mesh()->nodes().size();
			const uint32_t ns = model->analysis()->solver()->draw_steps();
			//mode
			m_mode = nn ? Mode::Mesh : Mode::Geometry;
			//allocate
			m_positions_data = new float[3 * nn * ns];
			m_rotations_data = new float[3 * nn * ns];
			model->analysis()->solver()->draw_positions(m_positions_data, m_scale);
			model->analysis()->solver()->draw_rotations(m_rotations_data, m_scale);
		}

		//destructor
		Draw::~Draw(void)
		{
			delete m_mesh;
			delete m_boundary;
			delete m_geometry;
			delete[] m_positions_data;
			delete[] m_rotations_data;
		}

		//data
		Mode Draw::mode(Mode mode)
		{
			return m_mode = mode;
		}
		Mode Draw::mode(void) const
		{
			return m_mode;
		}

		float Draw::scale(void) const
		{
			return m_scale;
		}
		float Draw::scale(float scale)
		{
			m_model->analysis()->solver()->draw_positions(m_positions_data, scale);
			m_model->analysis()->solver()->draw_rotations(m_rotations_data, scale);
			return m_scale = scale;
		}

		uint32_t Draw::step(void) const
		{
			return m_step;
		}
		uint32_t Draw::step(uint32_t step)
		{
			return m_step = step;
		}

		What& Draw::what(void)
		{
			return m_what;
		}
		const What& Draw::what(void) const
		{
			return m_what;
		}

		Sizes& Draw::sizes(void)
		{
			return m_sizes;
		}
		const Sizes& Draw::sizes(void) const
		{
			return m_sizes;
		}

		Colors& Draw::colors(void)
		{
			return m_colors;
		}
		const Colors& Draw::colors(void) const
		{
			return m_colors;
		}

		const canvas::cameras::BoundingBox& Draw::bounding_box(void) const
		{
			return m_mode == Mode::Geometry ? m_bounding_box_geometry : m_bounding_box_mesh;
		}

		//draw
		void Draw::draw(void)
		{
			if(m_mode != Mode::Geometry)
			{
				m_mesh->draw();
				m_boundary->draw();
			}
			else
			{
				m_geometry->draw();
			}
		}
		void Draw::setup(void)
		{
			if(m_mode != Mode::Geometry)
			{
				m_mesh->setup();
				m_boundary->setup();
			}
			else
			{
				m_geometry->setup();
			}
		}
		void Draw::update(void)
		{
			if(m_mode != Mode::Geometry)
			{
				m_mesh->update();
				m_boundary->update();
			}
			else
			{
				m_geometry->update();
			}
		}

		//compute
		void Draw::compute_bounding_box_mesh(void)
		{
			//data
			const uint32_t nn = m_model->mesh()->nodes().size();
			const uint32_t ns = m_model->analysis()->solver()->draw_steps();
			//positions
			for(uint32_t i = 0; i < ns; i++)
			{
				for(uint32_t j = 0; j < nn; j++)
				{
					m_bounding_box_mesh.insert_vertex(m_positions_data + 3 * nn * i + 3 * j);
				}
			}
			m_bounding_box_mesh.repair();
		}
		void Draw::compute_bounding_box_geometry(void)
		{
			for(const geometry::Point* point : m_model->geometry()->points())
			{
				m_bounding_box_geometry.insert_vertex(point->position());
			}
			m_bounding_box_geometry.repair();
		}

		//data
		const float* Draw::position(uint32_t index) const
		{
			const uint32_t nn = m_model->mesh()->nodes().size();
			return m_positions_data + 3 * nn * m_step + 3 * index;
		}
		const float* Draw::rotation(uint32_t index) const
		{
			const uint32_t nn = m_model->mesh()->nodes().size();
			return m_rotations_data + 3 * nn * m_step + 3 * index;
		}
	}
}
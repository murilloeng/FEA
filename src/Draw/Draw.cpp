//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"

#include "FEA/inc/Draw/Draw.hpp"
#include "FEA/inc/Draw/Mesh.hpp"
#include "FEA/inc/Draw/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace draw
	{
		//constructor
		Draw::Draw(const Model* model) : 
			m_mesh{new Mesh{this, model->mesh()}}, m_boundary{new Boundary{this, model->boundary()}}, m_model{model}, m_scale{1.0f}, m_step{0}
		{
			//data
			const uint32_t nn = model->mesh()->nodes().size();
			const uint32_t ns = model->analysis()->solver()->draw_steps();
			//allocate
			m_positions_data = new float[3 * nn * ns];
			m_rotations_data = new float[3 * nn * ns];
			model->analysis()->solver()->draw_positions(m_positions_data, m_scale);
			model->analysis()->solver()->draw_positions(m_rotations_data, m_scale);
		}
		
		//destructor
		Draw::~Draw(void)
		{
			delete m_mesh;
			delete m_boundary;
			delete[] m_positions_data;
			delete[] m_rotations_data;
		}

		//data
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

		Colors& Draw::colors(void)
		{
			return m_colors;
		}
		const Colors& Draw::colors(void) const
		{
			return m_colors;
		}

		//draw
		void Draw::draw(void)
		{
			m_mesh->draw();
			m_boundary->draw();
		}
		void Draw::setup(void)
		{
			m_mesh->setup();
			m_boundary->setup();
		}
		void Draw::update(void)
		{
			m_mesh->update();
			m_boundary->update();
		}

		//buonding box
		canvas::cameras::BoundingBox Draw::bounding_box(void) const
		{
			//data
			canvas::cameras::BoundingBox bounding_box;
			const uint32_t nn = m_model->mesh()->nodes().size();
			const uint32_t ns = m_model->analysis()->solver()->draw_steps();
			//positions
			for(uint32_t i = 0; i < ns; i++)
			{
				for(uint32_t j = 0; j < nn; j++)
				{
					bounding_box.insert_vertex(m_positions_data + 3 * nn * i + 3 * j);
				}
			}
			//return
			return bounding_box;
		}
	}
}
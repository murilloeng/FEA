//FEA
#include "FEA/inc/Model.hpp"
#include "FEA/inc/Draw/Draw.hpp"
#include "FEA/inc/Draw/Mesh.hpp"
#include "FEA/inc/Draw/Boundary.hpp"

namespace fea
{
	namespace draw
	{
		//constructor
		Draw::Draw(const Model* model) : 
			m_mesh{new Mesh{this, model->mesh()}}, m_boundary{new Boundary{this, model->boundary()}}
		{
			return;
		}
		
		//destructor
		Draw::~Draw(void)
		{
			delete m_mesh;
			delete m_boundary;
		}

		//data
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

		//update
		void Draw::update_bounding_box(canvas::cameras::BoundingBox& bounding_box) const
		{
			m_mesh->update_bounding_box(bounding_box);
			m_boundary->update_bounding_box(bounding_box);
		}
	}
}
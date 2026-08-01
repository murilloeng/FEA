#pragma once

//FEA
#include "FEA/inc/Draw/Colors.hpp"

//Canvas
#include "Canvas/inc/Objects/Object.hpp"
#include "Canvas/inc/Cameras/BoundingBox.hpp"

namespace fea
{
	class Model;
	namespace draw
	{
		class Mesh;
		class Engine;
		class Boundary;
	}
}

namespace fea
{
	namespace draw
	{
		class Draw : public canvas::objects::Object
		{
		public:
			//constructor
			Draw(const Model*);

			//destructor
			~Draw(void);

			//data
			float scale(float);
			float scale(void) const;

			uint32_t step(uint32_t);
			uint32_t step(void) const;

			Colors& colors(void);
			const Colors& colors(void) const;

		protected:
			//draw
			void draw(void) override;
			void setup(void) override;
			void update(void) override;

			//bounding box
			canvas::cameras::BoundingBox bounding_box(void) const;

			//data
			Colors m_colors;
			
			Mesh* m_mesh;
			Boundary* m_boundary;
			const Model* m_model;
			
			float m_scale;
			uint32_t m_step;
			float* m_positions_data;
			float* m_rotations_data;

			//friends
			friend class fea::draw::Mesh;
			friend class fea::draw::Engine;
			friend class fea::draw::Boundary;
		};
	}
}
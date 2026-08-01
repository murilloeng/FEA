#pragma once

//FEA
#include "FEA/inc/Draw/Colors.hpp"

//Canvas
#include "Canvas/inc/Objects/Object.hpp"

namespace fea
{
	class Model;
	namespace draw
	{
		class Mesh;
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
			Colors& colors(void);
			const Colors& colors(void) const;

		protected:
			//draw
			void draw(void) override;
			void setup(void) override;
			void update(void) override;

			//update
			void update_bounding_box(canvas::cameras::BoundingBox&) const override;

			//data
			Mesh* m_mesh;
			Colors m_colors;
			Boundary* m_boundary;
		};
	}
}
#pragma once

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

		protected:
			//draw
			void draw(void) override;
			void setup(void) override;
			void update(void) override;

			//data
			Mesh* m_mesh;
			Boundary* m_boundary;
		};
	}
}
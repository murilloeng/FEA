#pragma once

//FEA
#include "FEA/inc/Draw/Mode.hpp"
#include "FEA/inc/Draw/What.hpp"
#include "FEA/inc/Draw/Sizes.hpp"
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
		class Geometry;
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
			Mode mode(Mode);
			Mode mode(void) const;

			float scale(float);
			float scale(void) const;

			uint32_t step(uint32_t);
			uint32_t step(void) const;

			What& what(void);
			const What& what(void) const;

			Sizes& sizes(void);
			const Sizes& sizes(void) const;

			Colors& colors(void);
			const Colors& colors(void) const;

			const canvas::cameras::BoundingBox& bounding_box(void) const;

		protected:
			//draw
			void draw(void) override;
			void setup(void) override;
			void update(void) override;

			//compute
			void compute_bounding_box_mesh(void);
			void compute_bounding_box_geometry(void);

			//data
			const float* position(uint32_t) const;
			const float* rotation(uint32_t) const;

			//data
			Mode m_mode;
			What m_what;
			Sizes m_sizes;
			Colors m_colors;

			Mesh* m_mesh;
			Boundary* m_boundary;
			Geometry* m_geometry;
			const Model* m_model;

			float m_scale;
			uint32_t m_step;
			float* m_positions_data;
			float* m_rotations_data;

			canvas::cameras::BoundingBox m_bounding_box_mesh;
			canvas::cameras::BoundingBox m_bounding_box_geometry;

			//friends
			friend class fea::draw::Mesh;
			friend class fea::draw::Engine;
			friend class fea::draw::Boundary;
			friend class fea::draw::Geometry;
		};
	}
}
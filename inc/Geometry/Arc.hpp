#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>

//FEA
#include "FEA/inc/Geometry/Curve.hpp"

namespace fea
{
	namespace geometry
	{
		class Arc : public Curve
		{
		protected:
			//constructors
			Arc(uint32_t, uint32_t, uint32_t);

			//destructor
			virtual ~Arc(void);

		public:
			//data
			static uint32_t draw_mesh(void);
			static uint32_t draw_mesh(uint32_t);

		protected:
			//mesh
			void mesh(void) const override;

			//clone
			Curve* clone(void) const override;

			//draw
			virtual void draw_setup(draw::Data&) const;
			virtual void draw_update(draw::Data&) const;

			//data
			static uint32_t m_draw_mesh;

			//friends
			friend class fea::geometry::Geometry;
		};
	}
}
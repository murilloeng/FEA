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

		protected:
			//mesh
			void mesh(void) const override;

			//clone
			Curve* clone(void) const override;

			//friends
			friend class fea::geometry::Geometry;
		};
	}
}
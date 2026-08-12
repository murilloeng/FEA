//gmsh
#include <gmsh.h>

//FEA
#include "FEA/inc/Geometry/Arc.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Arc::Arc(uint32_t p1, uint32_t p2, uint32_t p3)
		{
			m_points.push_back(p1);
			m_points.push_back(p2);
			m_points.push_back(p3);
		}

		//destructor
		Arc::~Arc(void)
		{
			return;
		}

		//mesh
		void Arc::mesh(void) const
		{
			gmsh::model::geo::addCircleArc(m_points[0] + 1, m_points[1] + 1, m_points[2] + 1);
		}

		//clone
		Curve* Arc::clone(void) const
		{
			return new Arc(*this);
		}
	}
}
//gmsh
#include <gmsh.h>

//FEA
#include "FEA/inc/Geometry/Line.hpp"

namespace fea
{
	namespace geometry
	{
		//constructors
		Line::Line(uint32_t p1, uint32_t p2)
		{
			m_nodes.push_back(p1);
			m_nodes.push_back(p2);
		}

		//destructor
		Line::~Line(void)
		{
			return;
		}

		//mesh
		void Line::mesh(void) const
		{
			gmsh::model::geo::addLine(m_points[0] + 1, m_points[1] + 1);
		}
	}
}
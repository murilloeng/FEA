//std
#include <cstdlib>
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Line/Line.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			Line::Line(LoadCase* load_case) : Mechanic(load_case)
			{
				return;
			}

			//destructor
			Line::~Line(void)
			{
				return;
			}

			//serialization
			void Line::load(FILE* file)
			{
				Mechanic::load(file);
				if(fscanf(file, "%d", &m_edge) != 1)
				{
					throw std::runtime_error("Line load loading error!");
				}
			}
			void Line::save(FILE* file) const
			{
				Mechanic::save(file);
				if(fprintf(file, "%d ", m_edge) < 0)
				{
					throw std::runtime_error("Line load saving error!");
				}
			}

			//data
			uint32_t Line::edge(void) const
			{
				return m_edge;
			}
			uint32_t Line::edge(uint32_t edge)
			{
				return m_edge = edge;
			}
		}
	}
}
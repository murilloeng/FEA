//std
#include <cstdlib>
#include <stdexcept>

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Plane/Plane.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			Plane::Plane(LoadCase* load_case) : Mechanic(load_case), m_face(0)
			{
				return;
			}

			//destructor
			Plane::~Plane(void)
			{
				return;
			}

			//serialization
			void Plane::load(FILE* file)
			{
				Mechanic::load(file);
				if(fscanf(file, "%d", &m_face) != 1)
				{
					throw std::runtime_error("Plane load loading error!");
				}
			}
			void Plane::save(FILE* file) const
			{
				Mechanic::save(file);
				fprintf(file, "%02d ", m_face);
			}

			//data
			uint32_t Plane::face(void) const
			{
				return m_face;
			}
			uint32_t Plane::face(uint32_t face)
			{
				return m_face = face;
			}
		}
	}
}
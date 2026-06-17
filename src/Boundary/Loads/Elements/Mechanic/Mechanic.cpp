//std
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Mechanic.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			Mechanic::Mechanic(LoadCase* load_case) : Element(load_case), m_direction{1, 0, 0}
			{
				return;
			}

			//destructor
			Mechanic::~Mechanic(void)
			{
				return;
			}

			//serialization
			void Mechanic::load(FILE* file)
			{
				Element::load(file);
				if(fscanf(file, "%lf %lf %lf ", &m_direction[0], &m_direction[1], &m_direction[2]) != 3)
				{
					throw std::runtime_error("Mechanic load loading error!");
				}
			}
			void Mechanic::save(FILE* file) const
			{
				Element::save(file);
				fprintf(file, "%+.6e %+.6e %+.6e ", m_direction[0], m_direction[1], m_direction[2]);
			}

			//data
			double Mechanic::direction(uint32_t index) const
			{
				if(index > 2)
				{
					throw std::runtime_error("Mechanic load direction with index out of range!");
				}
				return m_direction[index];
			}
			const double* Mechanic::direction(void) const
			{
				return m_direction;
			}
			const double* Mechanic::direction(const double* direction)
			{
				memcpy(m_direction, direction, 3 * sizeof(double));
				math::Vector(m_direction, 3) /= math::Vector(m_direction, 3).norm();
				return m_direction;
			}
			const double* Mechanic::direction(double s1, double s2, double s3)
			{
				m_direction[0] = s1;
				m_direction[1] = s2;
				m_direction[2] = s3;
				math::Vector(m_direction, 3) /= math::Vector(m_direction, 3).norm();
				return m_direction;
			}
		}
	}
}
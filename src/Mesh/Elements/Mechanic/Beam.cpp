//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Beam.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam::Beam(void) : m_quadrature{2}
			{
				return;
			}

			//destructor
			Beam::~Beam(void)
			{
				return;
			}

			//data
			bool Beam::shear(void)
			{
				return m_shear;
			}
			bool Beam::shear(bool shear)
			{
				return m_shear = shear;
			}

			bool Beam::mixed(void)
			{
				return m_mixed;
			}
			bool Beam::mixed(bool mixed)
			{
				return m_mixed;
			}

			math::quadrature::Quadrature& Beam::quadrature(void)
			{
				return m_quadrature;
			}

			//static
			bool Beam::m_shear = false;
			bool Beam::m_mixed = false;
		}
	}
}
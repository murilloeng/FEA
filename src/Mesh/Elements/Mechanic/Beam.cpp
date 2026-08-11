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

			//analysis
			void Beam::setup(void)
			{
				Frame::setup();
				if(materials::Mechanic::inelastic())
				{
					m_points.reserve(m_quadrature.order());
					const uint32_t stresses = this->stress_set();
					for(uint32_t i = 0; i < m_quadrature.order(); i++)
					{
						m_points.push_back(points::Section(m_section));
						for(points::Fiber& fiber : m_points[i].fibers())
						{
							fiber.material_point().prepare(stresses);
						}
					}
				}
			}
			void Beam::update(void)
			{
				Frame::update();
				if(materials::Mechanic::inelastic())
				{
					for(points::Section& section : m_points)
					{
						for(points::Fiber& fiber : section.fibers())
						{
							fiber.material_point().update();
						}
					}
				}
			}
			void Beam::restore(void)
			{
				Frame::restore();
				if(materials::Mechanic::inelastic())
				{
					for(points::Section& section  : m_points)
					{
						for(points::Fiber& fiber : section.fibers())
						{
							fiber.material_point().restore();
						}
					}
				}
			}

			//static
			bool Beam::m_shear = false;
			bool Beam::m_mixed = false;
		}
	}
}
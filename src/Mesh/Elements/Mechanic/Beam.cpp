//std
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam::Beam(void) : m_major_axis{0, 0, 1}, m_quadrature{2}
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
				return m_mixed = mixed;
			}

			uint32_t Beam::draw_mesh(void)
			{
				return m_draw_mesh;
			}
			uint32_t Beam::draw_mesh(uint32_t draw_mesh)
			{
				return m_draw_mesh = draw_mesh;
			}

			math::quadrature::Quadrature& Beam::quadrature(void)
			{
				return m_quadrature;
			}

			const double* Beam::major_axis(void) const
			{
				return m_major_axis;
			}
			const double* Beam::major_axis(const double* major_axis)
			{
				return (const double*) memcpy(m_major_axis, major_axis, 3 * sizeof(double));
			}
			const double* Beam::major_axis(double s1, double s2, double s3)
			{
				m_major_axis[0] = s1;
				m_major_axis[1] = s2;
				m_major_axis[2] = s3;
				return m_major_axis;
			}

			//analysis
			void Beam::check(void)
			{
				//data
				Frame::check();
				const math::Vec3 z1 = node(0)->position();
				const math::Vec3 z2 = node(1)->position();
				//check
				const math::Vec3 s2(m_major_axis);
				const math::Vec3 s1 = (z2 - z1).unit();
				if(s1.cross(s2).norm() < 1.00e-05) throw std::runtime_error("Error: Beam element major axis is parallel to length!");
			}
			void Beam::setup(void)
			{
				//data
				Frame::setup();
				const math::Vec3 z1 = node(0)->position();
				const math::Vec3 z2 = node(1)->position();
				//projection
				const math::Vec3 s2(m_major_axis);
				const math::Vec3 s1 = (z2 - z1) / m_Lr;
				math::Vec3{m_major_axis} = (s2 - s1.inner(s2) * s1).unit();
				//points
				if(materials::Mechanic::inelastic())
				{
					m_points.clear();
					m_points.reserve(m_quadrature.order());
					const uint32_t stresses = this->stress_set();
					for(uint32_t i = 0; i < m_quadrature.order(); i++)
					{
						m_points.push_back(points::Section(m_section));
						for(points::Fiber& fiber : m_points[i].fibers())
						{
							fiber.material_point().setup(stresses);
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
			uint32_t Beam::m_draw_mesh = 20;
		}
	}
}
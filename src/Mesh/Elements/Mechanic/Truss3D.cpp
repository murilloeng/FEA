//std
#include <cmath>
#include <cstring>

//Math
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Mesh/Elements/Types.hpp"
#include "FEA/inc/Mesh/Elements/States.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss3D.hpp"

#include "FEA/inc/Boundary/Loads/Types.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Element.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Line/LineForce.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructors
			Truss3D::Truss3D(void)
			{
				return;
			}

			//destructor
			Truss3D::~Truss3D(void)
			{
				return;
			}

			//types
			Type Truss3D::type(void) const
			{
				return Type::Truss3D;
			}
			uint32_t Truss3D::dof_set(uint32_t) const
			{
				return
					(uint32_t) nodes::dof::translation_1 |
					(uint32_t) nodes::dof::translation_2 |
					(uint32_t) nodes::dof::translation_3;
			}

			//tangents
			void Truss3D::inertia(double* m) const
			{
				//data
				const double A = m_section->area();
				const double r = m_material->specific_mass();
				const math::Vec3 z0 = node(0)->coordinates();
				const math::Vec3 z1 = node(1)->coordinates();
				//inertia
				math::Matrix(m, 6, 6).zeros();
				const double Lr = (z1 - z0).norm();
				m[0 + 6 * 0] = m[1 + 6 * 1] = m[2 + 6 * 2] = m[3 + 6 * 3] = m[4 + 6 * 4] = m[5 + 6 * 5] = r * A * Lr / 3;
				m[0 + 6 * 3] = m[1 + 6 * 4] = m[2 + 6 * 5] = m[3 + 6 * 0] = m[4 + 6 * 1] = m[5 + 6 * 2] = r * A * Lr / 6;
			}
			void Truss3D::damping(double* c) const
			{
				memset(c, 0, 36 * sizeof(double));
			}
			void Truss3D::stiffness(double* k) const
			{
				//data
				math::Vec3 pi, pj;
				const math::Vec3 xi = node(0)->position(pi.data());
				const math::Vec3 xj = node(1)->position(pj.data());
				//stiffness
				const double Ln = (xj - xi).norm();
				const math::Vec3 t1 = (xj - xi) / Ln;
				k[0 + 6 * 0] = k[3 + 6 * 3] = +(m_K - m_f / Ln) * t1[0] * t1[0] + m_f / Ln;
				k[1 + 6 * 1] = k[4 + 6 * 4] = +(m_K - m_f / Ln) * t1[1] * t1[1] + m_f / Ln;
				k[2 + 6 * 2] = k[5 + 6 * 5] = +(m_K - m_f / Ln) * t1[2] * t1[2] + m_f / Ln;
				k[3 + 6 * 0] = k[0 + 6 * 3] = -(m_K - m_f / Ln) * t1[0] * t1[0] - m_f / Ln;
				k[4 + 6 * 1] = k[1 + 6 * 4] = -(m_K - m_f / Ln) * t1[1] * t1[1] - m_f / Ln;
				k[5 + 6 * 2] = k[2 + 6 * 5] = -(m_K - m_f / Ln) * t1[2] * t1[2] - m_f / Ln;
				k[1 + 6 * 0] = k[0 + 6 * 1] = k[4 + 6 * 3] = k[3 + 6 * 4] = +(m_K - m_f / Ln) * t1[0] * t1[1];
				k[2 + 6 * 0] = k[0 + 6 * 2] = k[5 + 6 * 3] = k[3 + 6 * 5] = +(m_K - m_f / Ln) * t1[0] * t1[2];
				k[2 + 6 * 1] = k[1 + 6 * 2] = k[5 + 6 * 4] = k[4 + 6 * 5] = +(m_K - m_f / Ln) * t1[1] * t1[2];
				k[4 + 6 * 0] = k[0 + 6 * 4] = k[3 + 6 * 1] = k[1 + 6 * 3] = -(m_K - m_f / Ln) * t1[0] * t1[1];
				k[5 + 6 * 0] = k[0 + 6 * 5] = k[3 + 6 * 2] = k[2 + 6 * 3] = -(m_K - m_f / Ln) * t1[0] * t1[2];
				k[5 + 6 * 1] = k[1 + 6 * 5] = k[4 + 6 * 2] = k[2 + 6 * 4] = -(m_K - m_f / Ln) * t1[1] * t1[2];
			}

			//force
			void Truss3D::internal_force(double* f) const
			{
				//data
				math::Vec3 pi, pj;
				const math::Vec3 xi = node(0)->position(pi.data());
				const math::Vec3 xj = node(1)->position(pj.data());
				const math::Vec3 t1 = (xj - xi).normalize();
				//internal force
				f[0] = -m_f * t1[0];
				f[1] = -m_f * t1[1];
				f[2] = -m_f * t1[2];
				f[3] = +m_f * t1[0];
				f[4] = +m_f * t1[1];
				f[5] = +m_f * t1[2];
			}

			//energy
			double Truss3D::kinetic_energy(void) const
			{
				return 0;
			}
			double Truss3D::internal_energy(void) const
			{
				return 0;
			}

			//loads
			void Truss3D::load_line_force(double* f, const boundary::loads::LineForce* load) const
			{
				//data
				const double p = load->value();
				const math::Vec3 sl = load->direction();
				const math::Vec3 z1 = node(0)->coordinates();
				const math::Vec3 z2 = node(1)->coordinates();
				const double L = (z2 - z1).norm();
				//external force
				f[0] = p * L * sl[0] / 2;
				f[1] = p * L * sl[1] / 2;
				f[2] = p * L * sl[2] / 2;
				f[3] = p * L * sl[0] / 2;
				f[4] = p * L * sl[1] / 2;
				f[5] = p * L * sl[2] / 2;
			}
		}
	}
}
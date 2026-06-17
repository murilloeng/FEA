//std
#include <cmath>
#include <cstring>

//Math
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Types.hpp"
#include "FEA/inc/Mesh/Elements/States.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"

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
			Truss2D::Truss2D(void)
			{
				return;
			}

			//destructor
			Truss2D::~Truss2D(void)
			{
				return;
			}

			//types
			Type Truss2D::type(void) const
			{
				return Type::Bar2D;
			}
			uint32_t Truss2D::dof_set(uint32_t) const
			{
				return
					(uint32_t) nodes::dof::translation_1 |
					(uint32_t) nodes::dof::translation_2;
			}

			//tangents
			void Truss2D::inertia(double* m) const
			{
				//data
				const double A = m_section->area();
				const double r = m_material->specific_mass();
				const math::Vec3 z0 = node(0)->coordinates();
				const math::Vec3 z1 = node(1)->coordinates();
				//inertia
				math::Matrix(m, 4, 4).zeros();
				const double Lr = (z1 - z0).norm();
				m[0 + 4 * 0] = m[1 + 4 * 1] = m[2 + 4 * 2] = m[3 + 4 * 3] = r * A * Lr / 3;
				m[0 + 4 * 2] = m[1 + 4 * 3] = m[2 + 4 * 0] = m[3 + 4 * 1] = r * A * Lr / 6;
			}
			void Truss2D::damping(double* c) const
			{
				memset(c, 0, 16 * sizeof(double));
			}
			void Truss2D::stiffness(double* k) const
			{
				//data
				math::Vec3 pi, pj;
				const math::Vec3 xi = node(0)->position(pi.data());
				const math::Vec3 xj = node(1)->position(pj.data());
				//stiffness
				const double Ln = (xj - xi).norm();
				const math::Vec3 t1 = (xj - xi) / Ln;
				k[0 + 4 * 0] = k[2 + 4 * 2] = +(m_K - m_f / Ln) * t1[0] * t1[0] + m_f / Ln;
				k[1 + 4 * 1] = k[3 + 4 * 3] = +(m_K - m_f / Ln) * t1[1] * t1[1] + m_f / Ln;
				k[2 + 4 * 0] = k[0 + 4 * 2] = -(m_K - m_f / Ln) * t1[0] * t1[0] - m_f / Ln;
				k[3 + 4 * 1] = k[1 + 4 * 3] = -(m_K - m_f / Ln) * t1[1] * t1[1] - m_f / Ln;
				k[1 + 4 * 0] = k[0 + 4 * 1] = k[3 + 4 * 2] = k[2 + 4 * 3] = +(m_K - m_f / Ln) * t1[0] * t1[1];
				k[3 + 4 * 0] = k[0 + 4 * 3] = k[2 + 4 * 1] = k[1 + 4 * 2] = -(m_K - m_f / Ln) * t1[0] * t1[1];
			}

			//forces
			void Truss2D::internal_force(double* f) const
			{
				//data
				math::Vec3 pi, pj;
				const math::Vec3 xi = node(0)->position(pi.data());
				const math::Vec3 xj = node(1)->position(pj.data());
				//direction
				math::Vec3 t1 = (xj - xi).normalize();
				//internal force
				f[0] = -m_f * t1[0];
				f[1] = -m_f * t1[1];
				f[2] = +m_f * t1[0];
				f[3] = +m_f * t1[1];
			}

			//energy
			double Truss2D::kinetic_energy(void) const
			{
				return 0;
			}
			double Truss2D::internal_energy(void) const
			{
				return 0;
			}

			//loads
			void Truss2D::load_line_force(double* f, const boundary::loads::LineForce* load) const
			{
				//data
				const double p = load->value();
				const math::Vec3 sl = load->direction();
				const math::Vec3 x1 = node(0)->coordinates();
				const math::Vec3 x2 = node(1)->coordinates();
				const double L = (x2 - x1).norm();
				//external force
				f[0] = p * L * sl[0] / 2;
				f[1] = p * L * sl[1] / 2;
				f[2] = p * L * sl[0] / 2;
				f[3] = p * L * sl[1] / 2;
			}
		}
	}
}
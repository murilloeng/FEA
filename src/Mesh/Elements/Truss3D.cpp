//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Truss3D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Truss3D::Truss3D(void)
			{
				return;
			}

			//destructor
			Truss3D::~Truss3D(void)
			{
				return;
			}

			//type
			Type Truss3D::type(void) const
			{
				return Type::Truss3D;
			}

			//dof
			uint32_t Truss3D::dof(uint32_t) const
			{
				return
					uint32_t(nodes::DOF::Translation_1)|
					uint32_t(nodes::DOF::Translation_2)|
					uint32_t(nodes::DOF::Translation_3);
			}

			//tangents
			void Truss3D::inertia(double*) const
			{
				return;
			}
			void Truss3D::damping(double*) const
			{
				return;
			}
			void Truss3D::stiffness(double* K) const
			{
				//data
				const math::Vec3 x0 = node(0)->position(true);
				const math::Vec3 x1 = node(1)->position(true);
				//stiffness
				const double Ln = (x1 - x0).norm();
				const math::Vec3 t1 = (x1 - x0) / Ln;
				K[0 + 6 * 0] = K[3 + 6 * 3] = +(m_K - m_f / Ln) * t1[0] * t1[0] + m_f / Ln;
				K[1 + 6 * 1] = K[4 + 6 * 4] = +(m_K - m_f / Ln) * t1[1] * t1[1] + m_f / Ln;
				K[2 + 6 * 2] = K[5 + 6 * 5] = +(m_K - m_f / Ln) * t1[2] * t1[2] + m_f / Ln;
				K[3 + 6 * 0] = K[0 + 6 * 3] = -(m_K - m_f / Ln) * t1[0] * t1[0] - m_f / Ln;
				K[4 + 6 * 1] = K[1 + 6 * 4] = -(m_K - m_f / Ln) * t1[1] * t1[1] - m_f / Ln;
				K[5 + 6 * 2] = K[2 + 6 * 5] = -(m_K - m_f / Ln) * t1[2] * t1[2] - m_f / Ln;
				K[1 + 6 * 0] = K[0 + 6 * 1] = K[4 + 6 * 3] = K[3 + 6 * 4] = +(m_K - m_f / Ln) * t1[0] * t1[1];
				K[2 + 6 * 0] = K[0 + 6 * 2] = K[5 + 6 * 3] = K[3 + 6 * 5] = +(m_K - m_f / Ln) * t1[0] * t1[2];
				K[2 + 6 * 1] = K[1 + 6 * 2] = K[5 + 6 * 4] = K[4 + 6 * 5] = +(m_K - m_f / Ln) * t1[1] * t1[2];
				K[4 + 6 * 0] = K[0 + 6 * 4] = K[3 + 6 * 1] = K[1 + 6 * 3] = -(m_K - m_f / Ln) * t1[0] * t1[1];
				K[5 + 6 * 0] = K[0 + 6 * 5] = K[3 + 6 * 2] = K[2 + 6 * 3] = -(m_K - m_f / Ln) * t1[0] * t1[2];
				K[5 + 6 * 1] = K[1 + 6 * 5] = K[4 + 6 * 2] = K[2 + 6 * 4] = -(m_K - m_f / Ln) * t1[1] * t1[2];
			}

			//forces
			void Truss3D::internal_force(double* f) const
			{
				//data
				const math::Vec3 x0 = node(0)->position(true);
				const math::Vec3 x1 = node(1)->position(true);
				//internal force
				const double Ln = (x1 - x0).norm();
				f[0] = -m_f * (x1[0] - x0[0]) / Ln;
				f[1] = -m_f * (x1[1] - x0[1]) / Ln;
				f[2] = -m_f * (x1[2] - x0[2]) / Ln;
				f[3] = +m_f * (x1[0] - x0[0]) / Ln;
				f[4] = +m_f * (x1[1] - x0[1]) / Ln;
				f[5] = +m_f * (x1[2] - x0[2]) / Ln;
			}
		}
	}
}
//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss3D.hpp"

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

			//data
			uint32_t Truss3D::dof_set(uint32_t) const
			{
				return 
					1 << uint32_t(nodes::DOF::Translation_1)|
					1 << uint32_t(nodes::DOF::Translation_2)|
					1 << uint32_t(nodes::DOF::Translation_3);
			}

			//tangents
			void Truss3D::inertia(double* M) const
			{
				math::Matrix(M, 6, 6).zeros();
			}
			void Truss3D::damping(double* C) const
			{
				math::Matrix(C, 6, 6).zeros();
			}
			void Truss3D::stiffness(double* K) const
			{
				//data
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//stiffness
				const math::Vec3 t1 = (x2 - x1) / m_Ln;
				K[0 + 6 * 0] = K[3 + 6 * 3] = +(m_K - m_f / m_Ln) * t1[0] * t1[0] + m_f / m_Ln;
				K[1 + 6 * 1] = K[4 + 6 * 4] = +(m_K - m_f / m_Ln) * t1[1] * t1[1] + m_f / m_Ln;
				K[2 + 6 * 2] = K[5 + 6 * 5] = +(m_K - m_f / m_Ln) * t1[2] * t1[2] + m_f / m_Ln;
				K[3 + 6 * 0] = K[0 + 6 * 3] = -(m_K - m_f / m_Ln) * t1[0] * t1[0] - m_f / m_Ln;
				K[4 + 6 * 1] = K[1 + 6 * 4] = -(m_K - m_f / m_Ln) * t1[1] * t1[1] - m_f / m_Ln;
				K[5 + 6 * 2] = K[2 + 6 * 5] = -(m_K - m_f / m_Ln) * t1[2] * t1[2] - m_f / m_Ln;
				K[1 + 6 * 0] = K[0 + 6 * 1] = K[4 + 6 * 3] = K[3 + 6 * 4] = +(m_K - m_f / m_Ln) * t1[0] * t1[1];
				K[2 + 6 * 0] = K[0 + 6 * 2] = K[5 + 6 * 3] = K[3 + 6 * 5] = +(m_K - m_f / m_Ln) * t1[0] * t1[2];
				K[2 + 6 * 1] = K[1 + 6 * 2] = K[5 + 6 * 4] = K[4 + 6 * 5] = +(m_K - m_f / m_Ln) * t1[1] * t1[2];
				K[4 + 6 * 0] = K[0 + 6 * 4] = K[3 + 6 * 1] = K[1 + 6 * 3] = -(m_K - m_f / m_Ln) * t1[0] * t1[1];
				K[5 + 6 * 0] = K[0 + 6 * 5] = K[3 + 6 * 2] = K[2 + 6 * 3] = -(m_K - m_f / m_Ln) * t1[0] * t1[2];
				K[5 + 6 * 1] = K[1 + 6 * 5] = K[4 + 6 * 2] = K[2 + 6 * 4] = -(m_K - m_f / m_Ln) * t1[1] * t1[2];

			}

			//forces
			void Truss3D::internal_force(double* f) const
			{
				//data
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//internal force
				f[0] = -m_f * (x2[0] - x1[0]) / m_Ln;
				f[1] = -m_f * (x2[1] - x1[1]) / m_Ln;
				f[2] = -m_f * (x2[2] - x1[2]) / m_Ln;
				f[3] = +m_f * (x2[0] - x1[0]) / m_Ln;
				f[4] = +m_f * (x2[1] - x1[1]) / m_Ln;
				f[5] = +m_f * (x2[2] - x1[2]) / m_Ln;

			}
		}
	}
}
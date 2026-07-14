//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Truss2D::Truss2D(void)
			{
				return;
			}

			//destructor
			Truss2D::~Truss2D(void)
			{
				return;
			}

			//data
			uint32_t Truss2D::dof_set(uint32_t) const
			{
				return 
					1 << uint32_t(nodes::DOF::Translation_1)|
					1 << uint32_t(nodes::DOF::Translation_2);
			}

			//tangents
			void Truss2D::inertia(double* M) const
			{
				math::Matrix(M, 4, 4).zeros();
			}
			void Truss2D::damping(double* C) const
			{
				math::Matrix(C, 4, 4).zeros();
			}
			void Truss2D::stiffness(double* K) const
			{
				//data
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//stiffness
				const math::Vec3 t1 = (x2 - x1) / m_Ln;
				K[0 + 4 * 0] = K[2 + 4 * 2] = +(m_K - m_f / m_Ln) * t1[0] * t1[0] + m_f / m_Ln;
				K[1 + 4 * 1] = K[3 + 4 * 3] = +(m_K - m_f / m_Ln) * t1[1] * t1[1] + m_f / m_Ln;
				K[2 + 4 * 0] = K[0 + 4 * 2] = -(m_K - m_f / m_Ln) * t1[0] * t1[0] - m_f / m_Ln;
				K[3 + 4 * 1] = K[1 + 4 * 3] = -(m_K - m_f / m_Ln) * t1[1] * t1[1] - m_f / m_Ln;
				K[1 + 4 * 0] = K[0 + 4 * 1] = K[3 + 4 * 2] = K[2 + 4 * 3] = +(m_K - m_f / m_Ln) * t1[0] * t1[1];
				K[3 + 4 * 0] = K[0 + 4 * 3] = K[2 + 4 * 1] = K[1 + 4 * 2] = -(m_K - m_f / m_Ln) * t1[0] * t1[1];

			}

			//forces
			void Truss2D::internal_force(double* f) const
			{
				//data
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//internal force
				f[0] = -m_f * (x2[0] - x1[0]) / m_Ln;
				f[1] = -m_f * (x2[1] - x1[1]) / m_Ln;
				f[2] = +m_f * (x2[0] - x1[0]) / m_Ln;
				f[3] = +m_f * (x2[1] - x1[1]) / m_Ln;
			}
		}
	}
}
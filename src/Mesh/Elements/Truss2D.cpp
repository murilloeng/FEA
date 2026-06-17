//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Type.hpp"
#include "FEA/inc/Mesh/Elements/Truss2D.hpp"

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

			//type
			Type Truss2D::type(void) const
			{
				return Type::Truss2D;
			}

			//dof
			uint32_t Truss2D::dof(uint32_t) const
			{
				return 
					uint32_t(DOF::Translation_1)|
					uint32_t(DOF::Translation_2);
			}

			//tangents
			void Truss2D::inertia(double*) const
			{
				return;
			}
			void Truss2D::damping(double*) const
			{
				return;
			}
			void Truss2D::stiffness(double* K) const
			{
				//data
				const math::Vec3 x0 = node(0)->position(true);
				const math::Vec3 x1 = node(1)->position(true);
				//stiffness
				const double Ln = (x1 - x0).norm();
				const math::Vec3 t1 = (x1 - x0) / Ln;
				K[0 + 4 * 0] = K[2 + 4 * 2] = +(m_k - m_f / Ln) * t1[0] * t1[0] + m_f / Ln;
				K[1 + 4 * 1] = K[3 + 4 * 3] = +(m_k - m_f / Ln) * t1[1] * t1[1] + m_f / Ln;
				K[2 + 4 * 0] = K[0 + 4 * 2] = -(m_k - m_f / Ln) * t1[0] * t1[0] - m_f / Ln;
				K[3 + 4 * 1] = K[1 + 4 * 3] = -(m_k - m_f / Ln) * t1[1] * t1[1] - m_f / Ln;
				K[1 + 4 * 0] = K[0 + 4 * 1] = K[3 + 4 * 2] = K[2 + 4 * 3] = +(m_k - m_f / Ln) * t1[0] * t1[1];
				K[3 + 4 * 0] = K[0 + 4 * 3] = K[2 + 4 * 1] = K[1 + 4 * 2] = -(m_k - m_f / Ln) * t1[0] * t1[1];
			}

			//forces
			void Truss2D::internal_force(double* f) const
			{
				//data
				const math::Vec3 x0 = node(0)->position(true);
				const math::Vec3 x1 = node(1)->position(true);
				//internal force
				const double Ln = (x1 - x0).norm();
				f[0] = -m_f * (x1[0] - x0[0]) / Ln;
				f[1] = -m_f * (x1[1] - x0[1]) / Ln;
				f[2] = +m_f * (x1[0] - x0[0]) / Ln;
				f[3] = +m_f * (x1[1] - x0[1]) / Ln;

			}
		}
	}
}
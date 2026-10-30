//Math
#include "Math/inc/Linear/Vec2.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"

//static
const fea::mesh::nodes::DOF du1 = fea::mesh::nodes::DOF::Translation_1;
const fea::mesh::nodes::DOF du2 = fea::mesh::nodes::DOF::Translation_2;

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
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//stiffness
				const math::Vec2 x1 = z1 + u1;
				const math::Vec2 x2 = z2 + u2;
				const math::Vec2 t1 = (x2 - x1) / m_Ln;
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
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//internal force
				const math::Vec2 x1 = z1 + u1;
				const math::Vec2 x2 = z2 + u2;
				f[0] = -m_f * (x2[0] - x1[0]) / m_Ln;
				f[1] = -m_f * (x2[1] - x1[1]) / m_Ln;
				f[2] = +m_f * (x2[0] - x1[0]) / m_Ln;
				f[3] = +m_f * (x2[1] - x1[1]) / m_Ln;
			}

			//energies
			double Truss2D::kinetic_energy(void) const
			{
				return 0;
			}
			double Truss2D::internal_energy(void) const
			{
				//data
				const double A = m_section->area();
				const double E = m_material->elastic_modulus();
				//data
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//strain
				const math::Vec2 x1 = z1 + u1;
				const math::Vec2 x2 = z2 + u2;
				const double Ln = (x2 - x1).norm();
				const double em = elements::strain_measure(m_strain_measure, Ln / m_Lr);
				//return
				return E * A * m_Lr / 2 * em * em;
			}
		}
	}
}
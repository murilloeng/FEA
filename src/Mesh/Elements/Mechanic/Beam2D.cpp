//std
#include <cmath>

//Math
#include "Math/inc/Linear/Vec3.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam2D::Beam2D(void)
			{
				return;
			}

			//destructor
			Beam2D::~Beam2D(void)
			{
				return;
			}

			//data
			uint32_t Beam2D::dof_set(uint32_t) const
			{
				return
					1 << uint32_t(nodes::DOF::Rotation_3)|
					1 << uint32_t(nodes::DOF::Translation_1)|
					1 << uint32_t(nodes::DOF::Translation_2);
			}

			//tangents
			void Beam2D::inertia(double* M) const
			{
				m_formulation == Formulation::Corotational ? inertia_CR(M) : inertia_TL(M);
			}
			void Beam2D::inertia_CR(double*) const
			{
				return;
			}
			void Beam2D::inertia_TL(double*) const
			{
				return;
			}

			void Beam2D::damping(double* C) const
			{
				m_formulation == Formulation::Corotational ? damping_CR(C) : damping_TL(C);
			}
			void Beam2D::damping_CR(double*) const
			{
				return;
			}
			void Beam2D::damping_TL(double*) const
			{
				return;
			}

			void Beam2D::stiffness(double* K) const
			{
				m_formulation == Formulation::Corotational ? stiffness_CR(K) : stiffness_TL(K);
			}
			void Beam2D::stiffness_CR(double*) const
			{
				return;
			}
			void Beam2D::stiffness_TL(double*) const
			{
				return;
			}

			//forces
			void Beam2D::internal_force(double* f) const
			{
				m_formulation == Formulation::Corotational ? internal_force_CR(f) : internal_force_TL(f);
			}
			void Beam2D::internal_force_CR(double*) const
			{
				return;
			}
			void Beam2D::internal_force_TL(double*) const
			{
				return;
			}

			//analysis
			void Beam2D::compute(void)
			{
				m_formulation == Formulation::Corotational ? compute_CR() : compute_TL();
			}
			void Beam2D::compute_CR(void)
			{
				compute_CR_state();
				m_formulation == Formulation::Corotational ? compute_CR_elastic() : compute_CR_plastic();
			}
			void Beam2D::compute_TL(void)
			{
				return;
			}

			//compute CR
			void Beam2D::compute_CR_state(void)
			{
				//kinematics
				const math::Vec3 z1 = node(0)->position_ref();
				const math::Vec3 z2 = node(1)->position_ref();
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//directions
				const double L0 = (z2 - z1).norm();
				const double Ln = (x2 - x1).norm();
				const math::Vector sn = (x2 - x1) / Ln;
				const math::Vector s0 = (z2 - z1) / L0;
				//rigid rotation
				const double tr_old = m_tr_old;
				const double cr_old = cos(tr_old);
				const double sr_old = sin(tr_old);
				const double cr_new = sn[0] * s0[0] + sn[1] * s0[1];
				const double sr_new = sn[1] * s0[0] - sn[0] * s0[1];
				const double cr_inc = cr_new * cr_old + sr_new * sr_old;
				const double sr_inc = sr_new * cr_old - cr_new * sr_old;
				//state
				m_tr_new = tr_old + atan2(sr_inc, cr_inc);
				const double t1 = node(0)->state(nodes::DOF::Rotation_3);
				const double t2 = node(1)->state(nodes::DOF::Rotation_3);
				//local state
				m_dl[0] = Ln - L0;
				m_dl[1] = t1 - m_tr_new;
				m_dl[2] = t2 - m_tr_new;
			}
			void Beam2D::compute_CR_elastic(void)
			{
				return;
			}
			void Beam2D::compute_CR_plastic(void)
			{
				return;
			}
		}
	}
}
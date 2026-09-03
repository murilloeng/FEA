//std
#include <cstring>

//Math
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Linear/Mat3.hpp"
#include "Math/inc/Linear/Quat.hpp"

//Materials
#include "Materials/inc/Mechanic/Stress.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam3D::Beam3D(void) : m_Kl(6, 6), m_dl(6), m_fl(6)
			{
				return;
			}

			//destructor
			Beam3D::~Beam3D(void)
			{
				return;
			}

			//data
			uint32_t Beam3D::stress_set(void) const
			{
				return m_shear ?
					1 << uint32_t(materials::Stress::Type::s11):
					1 << uint32_t(materials::Stress::Type::s11)|
					1 << uint32_t(materials::Stress::Type::s12)|
					1 << uint32_t(materials::Stress::Type::s13);
			}
			uint32_t Beam3D::dof_set(uint32_t) const
			{
				return
					1 << uint32_t(nodes::DOF::Rotation_1)|
					1 << uint32_t(nodes::DOF::Rotation_2)|
					1 << uint32_t(nodes::DOF::Rotation_3)|
					1 << uint32_t(nodes::DOF::Translation_1)|
					1 << uint32_t(nodes::DOF::Translation_2)|
					1 << uint32_t(nodes::DOF::Translation_3);
			}

			//tangents
			void Beam3D::inertia_CR(double* M) const
			{
				return;
			}
			void Beam3D::inertia_TL(double* M) const
			{
				return;
			}
			void Beam3D::inertia(double* M) const
			{
				m_formulation == Formulation::Corotational ? inertia_CR(M) : inertia_TL(M);
			}

			void Beam3D::damping_CR(double* C) const
			{
				return;
			}
			void Beam3D::damping_TL(double* C) const
			{
				return;
			}
			void Beam3D::damping(double* C) const
			{
				m_formulation == Formulation::Corotational ? damping_CR(C) : damping_TL(C);
			}

			void Beam3D::stiffness_CR(double* K) const
			{
				//data
				const math::Vec3 z1 = node(0)->position_ref();
				const math::Vec3 z2 = node(1)->position_ref();
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				const math::Vec3 t1 = node(0)->rotation_new();
				const math::Vec3 t2 = node(1)->rotation_new();
				//axes
				const math::Vec3 s1 = (z2 - z1) / m_Lr;
				const math::Vec3 s2 = m_major_axis;
				const math::Vec3 s3 = s1.cross(s2);
				//quaternions
				const math::Quat q0(s1, s2, s3);
				const math::Quat q1 = node(0)->quaternion_new();
				const math::Quat q2 = node(1)->quaternion_new();
				//data
				const math::Quat qr = q1 * q0;
				const math::Mat3 Rr = qr.rotation();
				const math::Mat3 Xr = (x2 - x1).spin();
				const math::Mat3 T1 = t1.rotation_gradient();
				const math::Mat3 T2 = t2.rotation_gradient();
				const math::Mat3 Rt = qr.conjugate().rotation();
				const math::Vec3 tl = qr.conjugate(q2 * q0).pseudo();
				const math::Mat3 Ti = tl.rotation_gradient_inverse();
				//material stiffness
				compute_CR_kinematic();
				math::Matrix(K, 12, 12) = m_B.transpose() * m_Kl * m_B;
				//geometric stiffness
				const math::Vec3 nl = m_fl.data() + 0;
				const math::Vec3 ml = m_fl.data() + 3;
				const math::Vec3 mp = Ti.transpose() * ml;
				const math::Mat3 Hl = tl.rotation_hessian_inverse(ml, true);
				math::Matrix(K, 12, 12).span(0, 3) += Rr * nl.spin() * Rt * T1;
				math::Matrix(K, 12, 12).span(6, 3) -= Rr * nl.spin() * Rt * T1;
				math::Matrix(K, 12, 12).span(9, 9) += t2.rotation_hessian(Rr * mp, true);
				math::Matrix(K, 12, 12).span(3, 0) -= T1.transpose() * Rr * nl.spin() * Rt;
				math::Matrix(K, 12, 12).span(3, 6) += T1.transpose() * Rr * nl.spin() * Rt;
				math::Matrix(K, 12, 12).span(3, 3) += T1.transpose() * Rr * Hl * Ti * Rt * T1;
				math::Matrix(K, 12, 12).span(9, 3) -= T2.transpose() * Rr * Hl * Ti * Rt * T1;
				math::Matrix(K, 12, 12).span(3, 9) -= T1.transpose() * Rr * Hl * Ti * Rt * T2;
				math::Matrix(K, 12, 12).span(9, 9) += T2.transpose() * Rr * Hl * Ti * Rt * T2;
				math::Matrix(K, 12, 12).span(9, 3) -= T2.transpose() * Rr * mp.spin() * Rt * T1;
				math::Matrix(K, 12, 12).span(3, 3) += T1.transpose() * Rr * mp.spin() * Rt * T1;
				math::Matrix(K, 12, 12).span(3, 3) += T1.transpose() * Xr * Rr * nl.spin() * Rt * T1;
				math::Matrix(K, 12, 12).span(3, 3) -= t1.rotation_hessian(Rr * mp + Xr * Rr * nl, true);
			}
			void Beam3D::stiffness_TL(double* K) const
			{
				return;
			}
			void Beam3D::stiffness(double* K) const
			{
				m_formulation == Formulation::Corotational ? stiffness_CR(K) : stiffness_TL(K);
			}

			//forces
			void Beam3D::internal_force_CR(double* f) const
			{
				compute_CR_kinematic();
				math::Vector(f, 12) = m_B.transpose() * m_fl;
			}
			void Beam3D::internal_force_TL(double* fi) const
			{
				return;
			}
			void Beam3D::internal_force(double* fi) const
			{
				m_formulation == Formulation::Corotational ? internal_force_CR(fi) : internal_force_TL(fi);
			}

			//analysis
			void Beam3D::update(void)
			{
				Beam::update();
			}
			void Beam3D::restore(void)
			{
				Beam::restore();
			}
			void Beam3D::compute(void)
			{
				m_formulation == Formulation::Corotational ? compute_CR() : compute_TL();
			}

			//compute
			void Beam3D::compute_CR(void)
			{
				compute_CR_state();
				materials::Mechanic::inelastic() ? compute_CR_plastic() : compute_CR_elastic();
			}
			void Beam3D::compute_TL(void)
			{
				return;
			}

			//compute CR
			void Beam3D::compute_CR_state(void)
			{
				//data
				const math::Vec3 z1 = node(0)->position_ref();
				const math::Vec3 z2 = node(1)->position_ref();
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//axes
				const math::Vec3 s1 = (z2 - z1) / m_Lr;
				const math::Vec3 s2 = m_major_axis;
				const math::Vec3 s3 = s1.cross(s2);
				//quaternions
				const math::Quat q0(s1, s2, s3);
				const math::Quat q1 = node(0)->quaternion_new();
				const math::Quat q2 = node(1)->quaternion_new();
				//local state
				const math::Quat qr = q1 * q0;
				m_dl.span(3, 0, 3, 1) = (qr.conjugate() * q2 * q0).pseudo();
				m_dl.span(0, 0, 3, 1) = qr.conjugate(x2 - x1) - math::Vec3(m_Lr, 0, 0);
			}
			void Beam3D::compute_CR_elastic(void)
			{
				//section
				const double A = m_section->area();
				const double I22 = m_section->inertia(0);
				const double I33 = m_section->inertia(1);
				const double A22 = m_section->shear_area(0);
				const double A33 = m_section->shear_area(1);
				const double J = m_section->torsion_constant();
				//material
				const double G = m_material->shear_modulus();
				const double E = m_material->elastic_modulus();
				//shear
				const double u2 = G * A22 * m_Lr * m_Lr / (G * A22 * m_Lr * m_Lr + 12 * E * I22);
				const double u3 = G * A33 * m_Lr * m_Lr / (G * A33 * m_Lr * m_Lr + 12 * E * I33);
				//force
				m_fl[0] = E * A / m_Lr * m_dl[0];
				m_fl[3] = G * J / m_Lr * m_dl[3];
				m_fl[1] = 6 * E * I22 * u2 / m_Lr / m_Lr * (2 * m_dl[1] / m_Lr - m_dl[5]);
				m_fl[2] = 6 * E * I33 * u3 / m_Lr / m_Lr * (2 * m_dl[2] / m_Lr + m_dl[4]);
				m_fl[4] = E * I33 / m_Lr * ((1 + 3 * u3) * m_dl[4] + 6 * u3 * m_dl[2] / m_Lr);
				m_fl[5] = E * I22 / m_Lr * ((1 + 3 * u2) * m_dl[5] - 6 * u2 * m_dl[1] / m_Lr);
				//stiffness
				m_Kl.zeros();
				m_Kl[0 + 6 * 0] = E * A / m_Lr;
				m_Kl[3 + 6 * 3] = G * J / m_Lr;
				m_Kl[4 + 6 * 4] = E * I33 * (1 + 3 * u3) / m_Lr;
				m_Kl[5 + 6 * 5] = E * I22 * (1 + 3 * u2) / m_Lr;
				m_Kl[1 + 6 * 1] = 12 * E * I22 * u2 / m_Lr / m_Lr / m_Lr;
				m_Kl[2 + 6 * 2] = 12 * E * I33 * u3 / m_Lr / m_Lr / m_Lr;
				m_Kl[1 + 6 * 5] = m_Kl[5 + 6 * 1] = -6 * E * I22 * u2 / m_Lr / m_Lr;
				m_Kl[2 + 6 * 4] = m_Kl[4 + 6 * 2] = +6 * E * I33 * u3 / m_Lr / m_Lr;
			}
			void Beam3D::compute_CR_plastic(void)
			{
				return;
			}
			void Beam3D::compute_CR_kinematic(void) const
			{
				//data
				const math::Vec3 t1 = node(0)->rotation_new();
				const math::Vec3 t2 = node(1)->rotation_new();
				const math::Vec3 z1 = node(0)->position_ref();
				const math::Vec3 z2 = node(1)->position_ref();
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//axes
				const math::Vec3 s1 = (z2 - z1) / m_Lr;
				const math::Vec3 s2 = m_major_axis;
				const math::Vec3 s3 = s1.cross(s2);
				//quaternions
				const math::Quat q0(s1, s2, s3);
				const math::Quat q1 = node(0)->quaternion_new();
				const math::Quat q2 = node(1)->quaternion_new();
				//data
				const math::Quat qr = q1 * q0;
				const math::Mat3 Xr = (x2 - x1).spin();
				const math::Mat3 T1 = t1.rotation_gradient();
				const math::Mat3 T2 = t2.rotation_gradient();
				const math::Mat3 Rt = qr.conjugate().rotation();
				const math::Vec3 tl = qr.conjugate(q2 * q0).pseudo();
				const math::Mat3 Ti = tl.rotation_gradient_inverse();
				//kinematic
				m_B.zeros();
				m_B.span(0, 0) = -Rt;
				m_B.span(0, 6) = +Rt;
				m_B.span(3, 3) = -Ti * Rt * T1;
				m_B.span(3, 9) = +Ti * Rt * T2;
				m_B.span(0, 3) = +Rt * Xr * T1;
			}
			void Beam3D::compute_CR_plastic_length(double* B, double s)
			{
				return;
			}
			void Beam3D::compute_CR_plastic_section(double* H, double x2, double x3)
			{
				return;
			}

			//static
			math::Matrix Beam3D::m_B(6, 12);
		}
	}
}
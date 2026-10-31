//std
#include <cmath>

//Math
#include "Math/inc/Linear/Vec2.hpp"
#include "Math/inc/Miscellaneous/rotations.hpp"

//Canvas
#include "Canvas/inc/Math/vec2.hpp"
#include "Canvas/inc/Math/mat2.hpp"
#include "Canvas/inc/Vertices/Model3D.hpp"

//Materials
#include "Materials/inc/Mechanic/Stress.hpp"

//FEA
#include "FEA/inc/Draw/Data.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

//static
const fea::mesh::nodes::DOF dt3 = fea::mesh::nodes::DOF::Rotation_3;
const fea::mesh::nodes::DOF du1 = fea::mesh::nodes::DOF::Translation_1;
const fea::mesh::nodes::DOF du2 = fea::mesh::nodes::DOF::Translation_2;

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam2D::Beam2D(void) : m_tr_old{0}, m_tr_new{0}
			{
				return;
			}

			//destructor
			Beam2D::~Beam2D(void)
			{
				return;
			}

			//data
			uint32_t Beam2D::stress_set(void) const
			{
				return m_shear ?
					1 << uint32_t(materials::Stress::Type::s11)|
					1 << uint32_t(materials::Stress::Type::s12):
					1 << uint32_t(materials::Stress::Type::s11);
			}
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
			void Beam2D::stiffness_CR(double* K) const
			{
				//data
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//direction
				const math::Vec2 x1 = z1 + u1;
				const math::Vec2 x2 = z2 + u2;
				const double Ln = (x2 - x1).norm();
				const double cn = (x2[0] - x1[0]) / Ln;
				const double sn = (x2[1] - x1[1]) / Ln;
				//gradient
				const math::Vector a({-cn, -sn, 0, cn, sn, 0});
				const math::Vector b({sn, -cn, 0, -sn, cn, 0});
				const math::Matrix B({
					{-cn, -sn, 0, +cn, +sn, 0},
					{-sn / Ln, +cn / Ln, 1, +sn / Ln, -cn / Ln, 0},
					{-sn / Ln, +cn / Ln, 0, +sn / Ln, -cn / Ln, 1}
				});
				//stiffness
				math::Matrix(K, 6, 6) = B.transpose() * math::Matrix(m_Kl, 3, 3) * B;
				math::Matrix(K, 6, 6) += m_fl[0] / Ln * b.outer() + (m_fl[1] + m_fl[2]) / Ln / Ln * (a.outer(b) + b.outer(a));
			}
			void Beam2D::stiffness_TL(double* K) const
			{
				double B[18], H[36];
				compute_TL_strains_hessian(H);
				compute_TL_strains_gradient(B);
				const math::Matrix Bm(B, 3, 6);
				const math::Matrix Hm(H, 6, 6);
				const math::Matrix Kl(m_Kl, 3, 3);
				math::Matrix(K, 6, 6) = m_Lr * (Bm.transpose() * Kl * Bm + Hm);
			}

			//forces
			void Beam2D::internal_force(double* f) const
			{
				m_formulation == Formulation::Corotational ? internal_force_CR(f) : internal_force_TL(f);
			}
			void Beam2D::internal_force_CR(double* f) const
			{
				//data
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//directions
				const math::Vec2 x1 = z1 + u1;
				const math::Vec2 x2 = z2 + u2;
				const double Ln = (x2 - x1).norm();
				const double cn = (x2[0] - x1[0]) / Ln;
				const double sn = (x2[1] - x1[1]) / Ln;
				//internal force
				f[2] = m_fl[1];
				f[5] = m_fl[2];
				f[0] = -cn * m_fl[0] - sn * (m_fl[1] + m_fl[2]) / Ln;
				f[1] = -sn * m_fl[0] + cn * (m_fl[1] + m_fl[2]) / Ln;
				f[3] = +cn * m_fl[0] + sn * (m_fl[1] + m_fl[2]) / Ln;
				f[4] = +sn * m_fl[0] - cn * (m_fl[1] + m_fl[2]) / Ln;
			}
			void Beam2D::internal_force_TL(double* f) const
			{
				double B[18];
				compute_TL_strains_gradient(B);
				math::Vector(f, 6) = m_Lr * math::Matrix(B, 3, 6).transpose() * math::Vector(m_fl, 3);
			}

			//energies
			double Beam2D::kinetic_energy_CR(void) const
			{
				return 0;
			}
			double Beam2D::kinetic_energy_TL(void) const
			{
				return 0;
			}
			double Beam2D::internal_energy_CR(void) const
			{
				return math::Matrix(m_Kl, 3, 3).bilinear(m_dl) / 2;
			}
			double Beam2D::internal_energy_TL(void) const
			{
				return m_Lr * math::Matrix(m_Kl, 3, 3).bilinear(m_dl) / 2;
			}
			double Beam2D::kinetic_energy(void) const
			{
				return m_formulation == Formulation::Corotational ? kinetic_energy_CR() : kinetic_energy_TL();
			}
			double Beam2D::internal_energy(void) const
			{
				return m_formulation == Formulation::Corotational ? internal_energy_CR() : internal_energy_TL();
			}

			//analysis
			void Beam2D::update(void)
			{
				Beam::update();
				m_tr_old = m_tr_new;
			}
			void Beam2D::restore(void)
			{
				Beam::restore();
				m_tr_new = m_tr_old;
			}
			void Beam2D::compute(void)
			{
				m_formulation == Formulation::Corotational ? compute_CR() : compute_TL();
			}

			//compute
			void Beam2D::compute_CR(void)
			{
				compute_CR_state();
				materials::Mechanic::inelastic() ? compute_CR_plastic() : compute_CR_elastic();
			}
			void Beam2D::compute_TL(void)
			{
				compute_TL_state();
				compute_TL_elastic();
			}

			//compute TL
			void Beam2D::compute_TL_state(void)
			{
				//data
				const double t1 = node(0)->state(dt3);
				const double t2 = node(1)->state(dt3);
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//rotation
				const double dz1 = z2[0] - z1[0];
				const double dz2 = z2[1] - z1[1];
				const double a1 = atan2(dz2, dz1) + t1;
				const double a2 = atan2(dz2, dz1) + t2;
				//rotation
				const double kp = a2 - a1;
				const double fk = math::fn(kp, 1) / math::fn(kp, 2) / 2;
				const double b1 = (z2[0] + u2[0] - z1[0] - u1[0]) / m_Lr;
				const double b2 = (z2[1] + u2[1] - z1[1] - u1[1]) / m_Lr;
				//functions
				const double g1 = fk * cos(a1) - kp / 2 * sin(a1);
				const double g2 = fk * sin(a1) + kp / 2 * cos(a1);
				//strains
				m_dl[2] = kp / m_Lr;
				m_dl[1] = g1 * b2 - g2 * b1;
				m_dl[0] = g1 * b1 + g2 * b2 - 1;
			}
			void Beam2D::compute_TL_elastic(void)
			{
				//material
				const double G = m_material->shear_modulus();
				const double E = m_material->elastic_modulus();
				//section
				const double A = m_section->area();
				const double I33 = m_section->inertia(1);
				const double A22 = m_shear ? m_section->shear_area(0) : 1e+5 * E * I33 / m_Lr / m_Lr / G;
				//resultants
				m_fl[0] = E * A * m_dl[0];
				m_fl[1] = G * A22 * m_dl[1];
				m_fl[2] = E * I33 * m_dl[2];
				//stiffness
				m_Kl[0] = E * A;
				m_Kl[4] = G * A22;
				m_Kl[8] = E * I33;
				m_Kl[1] = m_Kl[2] = m_Kl[3] = 0;
				m_Kl[5] = m_Kl[6] = m_Kl[7] = 0;
			}
			void Beam2D::compute_TL_strains_hessian(double* H) const
			{
				//data
				const double t1 = node(0)->state(dt3);
				const double t2 = node(1)->state(dt3);
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//rotation
				const double dz1 = z2[0] - z1[0];
				const double dz2 = z2[1] - z1[1];
				const double a1 = atan2(dz2, dz1) + t1;
				const double a2 = atan2(dz2, dz1) + t2;
				//rotation
				const double kp = a2 - a1;
				const double f1 = math::fn(kp, 1);
				const double f2 = math::fn(kp, 2);
				const double f3 = math::fn(kp, 3);
				const double d2 = math::dfn(kp, 2);
				const double d3 = math::dfn(kp, 3);
				const double b1 = (z2[0] + u2[0] - z1[0] - u1[0]) / m_Lr;
				const double b2 = (z2[1] + u2[1] - z1[1] - u1[1]) / m_Lr;
				//rotation
				const double qk = f1 / f2 / 2;
				const double dq = -kp * f3 / f2 / 2;
				const double hq = (kp * (f3 * d2 - f2 * d3) - f2 * f3) / f2 / f2 / 2;
				//functions
				const double g1 = qk * cos(a1) - kp / 2 * sin(a1);
				const double g2 = qk * sin(a1) + kp / 2 * cos(a1);
				//gradient 2
				const double dg12 = dq * cos(a1) - sin(a1) / 2;
				const double dg22 = dq * sin(a1) + cos(a1) / 2;
				//gradient 1
				const double dg11 = -g2 - dg12;
				const double dg21 = +g1 - dg22;
				//hessian 1
				const double dg122 = hq * cos(a1);
				const double dg222 = hq * sin(a1);
				const double dg121 = -hq * cos(a1) - dq * sin(a1) - cos(a1) / 2;
				const double dg221 = -hq * sin(a1) + dq * cos(a1) - sin(a1) / 2;
				//hessian 2
				const double dg111 = -dg21 - dg121;
				const double dg211 = +dg11 - dg221;
				const double dg112 = -dg22 - dg122;
				const double dg212 = +dg12 - dg222;
				//gradient
				H[0 + 6 * 0] = H[0 + 6 * 1] = H[0 + 6 * 3] = H[0 + 6 * 4] = 0;
				H[1 + 6 * 0] = H[1 + 6 * 1] = H[1 + 6 * 3] = H[1 + 6 * 4] = 0;
				H[3 + 6 * 0] = H[3 + 6 * 1] = H[3 + 6 * 3] = H[3 + 6 * 4] = 0;
				H[4 + 6 * 0] = H[4 + 6 * 1] = H[4 + 6 * 3] = H[4 + 6 * 4] = 0;
				H[0 + 6 * 2] = H[2 + 6 * 0] = (-dg11 * m_fl[0] + dg21 * m_fl[1]) / m_Lr;
				H[0 + 6 * 5] = H[5 + 6 * 0] = (-dg12 * m_fl[0] + dg22 * m_fl[1]) / m_Lr;
				H[1 + 6 * 2] = H[2 + 6 * 1] = (-dg21 * m_fl[0] - dg11 * m_fl[1]) / m_Lr;
				H[1 + 6 * 5] = H[5 + 6 * 1] = (-dg22 * m_fl[0] - dg12 * m_fl[1]) / m_Lr;
				H[2 + 6 * 3] = H[3 + 6 * 2] = (+dg11 * m_fl[0] - dg21 * m_fl[1]) / m_Lr;
				H[2 + 6 * 4] = H[4 + 6 * 2] = (+dg21 * m_fl[0] + dg11 * m_fl[1]) / m_Lr;
				H[3 + 6 * 5] = H[5 + 6 * 3] = (+dg12 * m_fl[0] - dg22 * m_fl[1]) / m_Lr;
				H[4 + 6 * 5] = H[5 + 6 * 4] = (+dg22 * m_fl[0] + dg12 * m_fl[1]) / m_Lr;
				H[2 + 6 * 2] = (dg111 * b1 + dg211 * b2) * m_fl[0] + (dg111 * b2 - dg211 * b1) * m_fl[1];
				H[2 + 6 * 5] = (dg112 * b1 + dg212 * b2) * m_fl[0] + (dg112 * b2 - dg212 * b1) * m_fl[1];
				H[5 + 6 * 2] = (dg121 * b1 + dg221 * b2) * m_fl[0] + (dg121 * b2 - dg221 * b1) * m_fl[1];
				H[5 + 6 * 5] = (dg122 * b1 + dg222 * b2) * m_fl[0] + (dg122 * b2 - dg222 * b1) * m_fl[1];
			}
			void Beam2D::compute_TL_strains_gradient(double* B) const
			{
				//data
				const double t1 = node(0)->state(dt3);
				const double t2 = node(1)->state(dt3);
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//rotation
				const double dz1 = z2[0] - z1[0];
				const double dz2 = z2[1] - z1[1];
				const double a1 = atan2(dz2, dz1) + t1;
				const double a2 = atan2(dz2, dz1) + t2;
				//rotation
				const double kp = a2 - a1;
				const double fk = math::fn(kp, 1) / math::fn(kp, 2) / 2;
				const double b1 = (z2[0] + u2[0] - z1[0] - u1[0]) / m_Lr;
				const double b2 = (z2[1] + u2[1] - z1[1] - u1[1]) / m_Lr;
				const double df = -kp * math::fn(kp, 3) / math::fn(kp, 2) / 2;
				//functions
				const double g1 = fk * cos(a1) - kp / 2 * sin(a1);
				const double g2 = fk * sin(a1) + kp / 2 * cos(a1);
				//gradient 2
				const double dg12 = df * cos(a1) - sin(a1) / 2;
				const double dg22 = df * sin(a1) + cos(a1) / 2;
				//gradient 1
				const double dg11 = -g2 - dg12;
				const double dg21 = +g1 - dg22;
				//gradient
				B[2 + 3 * 2] = -1 / m_Lr;
				B[2 + 3 * 5] = +1 / m_Lr;
				B[0 + 3 * 2] = dg11 * b1 + dg21 * b2;
				B[1 + 3 * 2] = dg11 * b2 - dg21 * b1;
				B[0 + 3 * 5] = dg12 * b1 + dg22 * b2;
				B[1 + 3 * 5] = dg12 * b2 - dg22 * b1;
				B[0 + 3 * 0] = B[1 + 3 * 1] = -g1 / m_Lr;
				B[1 + 3 * 0] = B[0 + 3 * 4] = +g2 / m_Lr;
				B[0 + 3 * 1] = B[1 + 3 * 3] = -g2 / m_Lr;
				B[0 + 3 * 3] = B[1 + 3 * 4] = +g1 / m_Lr;
				B[2 + 3 * 0] = B[2 + 3 * 1] = B[2 + 3 * 3] = B[2 + 3 * 4] = 0;
			}

			//compute CR
			void Beam2D::compute_CR_state(void)
			{
				//data
				const double t1 = node(0)->state(dt3);
				const double t2 = node(1)->state(dt3);
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//directions
				const math::Vec2 x1 = z1 + u1;
				const math::Vec2 x2 = z2 + u2;
				const double Ln = (x2 - x1).norm();
				const math::Vec2 sn = (x2 - x1) / Ln;
				const math::Vec2 s0 = (z2 - z1) / m_Lr;
				//rigid rotation
				const double cr_old = cos(m_tr_old);
				const double sr_old = sin(m_tr_old);
				const double cr_new = sn[0] * s0[0] + sn[1] * s0[1];
				const double sr_new = sn[1] * s0[0] - sn[0] * s0[1];
				const double cr_inc = cr_new * cr_old + sr_new * sr_old;
				const double sr_inc = sr_new * cr_old - cr_new * sr_old;
				//state
				m_tr_new = m_tr_old + atan2(sr_inc, cr_inc);
				//local state
				m_dl[0] = Ln - m_Lr;
				m_dl[1] = t1 - m_tr_new;
				m_dl[2] = t2 - m_tr_new;
			}
			void Beam2D::compute_CR_elastic(void)
			{
				//data
				const double u2 = m_dl[0];
				const double t1 = m_dl[1];
				const double t2 = m_dl[2];
				//section
				const double A = m_section->area();
				const double I33 = m_section->inertia(1);
				const double A22 = m_section->shear_area(0);
				//material
				const double G = m_material->shear_modulus();
				const double E = m_material->elastic_modulus();
				//shear
				const double q = G * A22 * m_Lr * m_Lr;
				const double w = !m_shear ? 0 : E * I33 / q;
				const double m = !m_shear ? 1 : 1 / (1 + 12 * w);
				//force
				m_fl[0] = E * A * u2 / m_Lr;
				m_fl[1] = E * I33 / m_Lr * m * (4 * (1 + 3 * w) * t1 + 2 * (1 - 6 * w) * t2);
				m_fl[2] = E * I33 / m_Lr * m * (4 * (1 + 3 * w) * t2 + 2 * (1 - 6 * w) * t1);
				//stiffness
				m_Kl[0 + 3 * 0] = E * A / m_Lr;
				m_Kl[2 + 3 * 1] = m_Kl[1 + 3 * 2] = 2 * E * I33 / m_Lr * m * (1 - 6 * w);
				m_Kl[1 + 3 * 1] = m_Kl[2 + 3 * 2] = 4 * E * I33 / m_Lr * m * (1 + 3 * w);
				m_Kl[1 + 3 * 0] = m_Kl[0 + 3 * 1] = m_Kl[2 + 3 * 0] = m_Kl[0 + 3 * 2] = 0;
			}
			void Beam2D::compute_CR_plastic(void)
			{
				//data
				const math::Vector dl(m_dl, 3);
				const uint32_t np = m_shear ? 2 : 1;
				const uint32_t ns = m_shear ? 3 : 2;
				math::Vector fl(m_fl, 3), ep(np), sp(np), es(ns), ss(ns);
				math::Matrix Kl(m_Kl, 3, 3), B(ns, 3), H(np, ns), Kp(np, np), Ks(ns, ns);
				//compute
				fl.zeros();
				Kl.zeros();
				for(uint32_t i = 0; i < m_quadrature.order(); i++)
				{
					//quadrature
					const double s = m_quadrature.point(i);
					const double w = m_quadrature.weight(i);
					compute_CR_plastic_length(B.data(), s);
					//fibers
					ss.zeros();
					Ks.zeros();
					es = B * dl;
					for(points::Fiber& fiber : m_points[i].fibers())
					{
						//fiber
						const double A = fiber.area();
						const double x2 = fiber.position(0);
						compute_CR_plastic_section(H.data(), x2);
						//material
						ep = H * es;
						m_material->return_mapping(sp.data(), Kp.data(), ep.data(), fiber.material_point());
						//contribution
						ss += A * H.transpose() * sp;
						Ks += A * H.transpose() * Kp * H;
					}
					//contribution
					fl += w * m_Lr / 2 * B.transpose() * ss;
					Kl += w * m_Lr / 2 * B.transpose() * Ks * B;
				}
			}
			void Beam2D::compute_CR_plastic_length(double* B, double s)
			{
				//data
				const double a = (1 + s) / 2;
				//kinematics
				if(!m_shear)
				{
					B[0 + 2 * 0] = 1 / m_Lr;
					B[1 + 2 * 1] = (6 * a - 4) / m_Lr;
					B[1 + 2 * 2] = (6 * a - 2) / m_Lr;
					B[1 + 2 * 0] = B[0 + 2 * 1] = B[0 + 2 * 2] = 0;
				}
			}
			void Beam2D::compute_CR_plastic_section(double* H, double x2)
			{
				if(!m_shear)
				{
					H[0 + 1 * 0] = 1;
					H[0 + 1 * 1] = -x2;
				}
			}

			//draw
			void Beam2D::draw_setup(draw::Data& data) const
			{
				data.m_counter_edges += 2 * m_draw_mesh;
				data.m_counter_vertices += m_draw_mesh + 1;
			}
			void Beam2D::draw_update(draw::Data& data) const
			{
				//data
				const uint32_t nn = m_mesh->nodes().size();
				uint32_t* ibo_ptr = data.m_ibo.data() + data.m_counter_dots + data.m_index_edges;
				const float t1 = data.m_rotations_data[3 * nn * data.m_step + 3 * m_nodes[0] + 2];
				const float t2 = data.m_rotations_data[3 * nn * data.m_step + 3 * m_nodes[1] + 2];
				const canvas::vec2 x1 = data.m_positions_data + 3 * nn * data.m_step + 3 * m_nodes[0];
				const canvas::vec2 x2 = data.m_positions_data + 3 * nn * data.m_step + 3 * m_nodes[1];
				canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) data.m_vbo.data() + data.m_index_vertices;
				//ibo data
				for(uint32_t i = 0; i < m_draw_mesh; i++)
				{
					ibo_ptr[2 * i + 0] = data.m_index_vertices + i + 0;
					ibo_ptr[2 * i + 1] = data.m_index_vertices + i + 1;
				}
				//vbo data
				const float tr = t2 - t1;
				for(uint32_t i = 0; i <= m_draw_mesh; i++)
				{
					//data
					const float s = float(i) / m_draw_mesh;
					const float fc = fabs(1 - cos(tr)) > 1e-5 ? (cos((1 - s) * tr) - cos(s * tr)) / (1 - cos(tr)) : 2 * s - 1;
					const float fs = fabs(1 - cos(tr)) > 1e-5 ? (sin(tr) - sin((1 - s) * tr) - sin(s * tr)) / (1 - cos(tr)) : 0;
					//vbo data
					vbo_ptr[i].m_color = data.m_colors.elements();
					vbo_ptr[i].m_position = (x1 + x2) / 2 + canvas::mat2{fc, fs, -fs, fc} * (x2 - x1) / 2;
				}
				//update
				data.m_index_edges += 2 * m_draw_mesh;
				data.m_index_vertices += m_draw_mesh + 1;
			}
		}
	}
}
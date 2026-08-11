//std
#include <cmath>

//Math
#include "Math/inc/Linear/Vec3.hpp"

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
			uint32_t Beam2D::draw_mesh(void)
			{
				return m_draw_mesh;
			}
			uint32_t Beam2D::draw_mesh(uint32_t draw_mesh)
			{
				return m_draw_mesh = draw_mesh;
			}

			uint32_t Beam2D::stress_set(void) const
			{
				return m_shear ?
					1 << uint32_t(materials::Stress::Type::s11):
					1 << uint32_t(materials::Stress::Type::s11)|
					1 << uint32_t(materials::Stress::Type::s12);
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
			void Beam2D::stiffness_CR(double* K) const
			{
				//data
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//direction
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
			void Beam2D::stiffness_TL(double*) const
			{
				return;
			}

			//forces
			void Beam2D::internal_force(double* f) const
			{
				m_formulation == Formulation::Corotational ? internal_force_CR(f) : internal_force_TL(f);
			}
			void Beam2D::internal_force_CR(double* f) const
			{
				//data
				const math::Vec3 x1 = node(0)->position_new();
				const math::Vec3 x2 = node(1)->position_new();
				//directions
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
			void Beam2D::internal_force_TL(double*) const
			{
				return;
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

			//analysis
			void Beam2D::compute(void)
			{
				m_formulation == Formulation::Corotational ? compute_CR() : compute_TL();
			}
			void Beam2D::compute_CR(void)
			{
				compute_CR_state();
				materials::Mechanic::inelastic() ? compute_CR_plastic() : compute_CR_elastic();
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
				const double cr_old = cos(m_tr_old);
				const double sr_old = sin(m_tr_old);
				const double cr_new = sn[0] * s0[0] + sn[1] * s0[1];
				const double sr_new = sn[1] * s0[0] - sn[0] * s0[1];
				const double cr_inc = cr_new * cr_old + sr_new * sr_old;
				const double sr_inc = sr_new * cr_old - cr_new * sr_old;
				//state
				m_tr_new = m_tr_old + atan2(sr_inc, cr_inc);
				const double t1 = node(0)->state(nodes::DOF::Rotation_3);
				const double t2 = node(1)->state(nodes::DOF::Rotation_3);
				//local state
				m_dl[0] = Ln - L0;
				m_dl[1] = t1 - m_tr_new;
				m_dl[2] = t2 - m_tr_new;
			}
			void Beam2D::compute_CR_elastic(void)
			{
				//dof
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
				//local stiffness
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
					compute_CR_plastic_kinematic(B.data(), s);
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
			void Beam2D::compute_CR_plastic_section(double* H, double x2)
			{
				if(!m_shear)
				{
					H[0 + 1 * 0] = 1;
					H[0 + 1 * 1] = -x2;
				}
			}
			void Beam2D::compute_CR_plastic_kinematic(double* B, double s)
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

			//static
			uint32_t Beam2D::m_draw_mesh = 20;
		}
	}
}
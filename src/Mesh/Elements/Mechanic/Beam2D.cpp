//std
#include <cmath>

//Math
#include "Math/inc/Linear/Vec2.hpp"

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
			uint32_t Beam2D::stress_set(void) const
			{
				return m_shear ?
					1 << uint32_t(materials::Stress::Type::s11):
					1 << uint32_t(materials::Stress::Type::s11)|
					1 << uint32_t(materials::Stress::Type::s12);
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
				const math::Vec2 z1 = node(0)->position_ref();
				const math::Vec2 z2 = node(1)->position_ref();
				const double t1 = node(0)->state(nodes::DOF::Rotation_3);
				//CR system
				const double yl = m_dl[1];
				const double xl = m_dl[0] + m_Lr;
				const double c0 = (z2[0] - z1[0]) / m_Lr;
				const double s0 = (z2[1] - z1[1]) / m_Lr;
				const double cr = c0 * cos(t1) - s0 * sin(t1);
				const double sr = s0 * cos(t1) + c0 * sin(t1);
				//gradient
				const math::Matrix B({
					{-cr, -sr, +yl, +cr, +sr, 0},
					{+sr, -cr, -xl, -sr, +cr, 0},
					{0, 0, -1, 0, 0, +1}
				});
				//hessian
				const math::Matrix H0({
					{0, 0, +sr, 0, 0, 0},
					{0, 0, -cr, 0, 0, 0},
					{+sr, -cr, -xl, -sr, +cr, 0},
					{0, 0, -sr, 0, 0, 0},
					{0, 0, +cr, 0, 0, 0},
					{0, 0, 0, 0, 0, 0}
				});
				const math::Matrix H1({
					{0, 0, +cr, 0, 0, 0},
					{0, 0, +sr, 0, 0, 0},
					{+cr, +sr, -yl, -cr, -sr, 0},
					{0, 0, -cr, 0, 0, 0},
					{0, 0, -sr, 0, 0, 0},
					{0, 0, 0, 0, 0, 0}
				});
				//stiffness
				math::Matrix(K, 6, 6) = B.transpose() * math::Matrix(m_Kl, 3, 3) * B + m_fl[0] * H0 + m_fl[1] * H1;
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
				const math::Vec2 z1 = node(0)->position_ref();
				const math::Vec2 z2 = node(1)->position_ref();
				const double t1 = node(0)->state(nodes::DOF::Rotation_3);
				//CR system
				const double yl = m_dl[1];
				const double xl = m_dl[0] + m_Lr;
				const double c0 = (z2[0] - z1[0]) / m_Lr;
				const double s0 = (z2[1] - z1[1]) / m_Lr;
				const double cr = c0 * cos(t1) - s0 * sin(t1);
				const double sr = s0 * cos(t1) + c0 * sin(t1);
				//internal force
				f[5] = +m_fl[2];
				f[0] = -cr * m_fl[0] + sr * m_fl[1];
				f[1] = -sr * m_fl[0] - cr * m_fl[1];
				f[3] = +cr * m_fl[0] - sr * m_fl[1];
				f[4] = +sr * m_fl[0] + cr * m_fl[1];
				f[2] = -m_fl[2] + yl * m_fl[0] - xl * m_fl[1];
			}
			void Beam2D::internal_force_TL(double*) const
			{
				return;
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
				return 0;
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
				return;
			}

			//compute CR
			void Beam2D::compute_CR_state(void)
			{
				//data
				const math::Vec2 z1 = node(0)->position_ref();
				const math::Vec2 z2 = node(1)->position_ref();
				const math::Vec2 x1 = node(0)->position_new();
				const math::Vec2 x2 = node(1)->position_new();
				const double t1 = node(0)->state(nodes::DOF::Rotation_3);
				const double t2 = node(1)->state(nodes::DOF::Rotation_3);
				//CR system
				const double c0 = (z2[0] - z1[0]) / m_Lr;
				const double s0 = (z2[1] - z1[1]) / m_Lr;
				const double cr = c0 * cos(t1) - s0 * sin(t1);
				const double sr = s0 * cos(t1) + c0 * sin(t1);
				//local state
				m_dl[2] = t2 - t1;
				m_dl[1] = cr * (x2[1] - x1[1]) - sr * (x2[0] - x1[0]);
				m_dl[0] = cr * (x2[0] - x1[0]) + sr * (x2[1] - x1[1]) - m_Lr;
			}
			void Beam2D::compute_CR_elastic(void)
			{
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
				m_fl[0] = E * A / m_Lr * m_dl[0];
				m_fl[1] = 6 * m * E * I33 / m_Lr / m_Lr * (2 * m_dl[1] / m_Lr - m_dl[2]);
				m_fl[2] = 2 * m * E * I33 / m_Lr * (2 * (1 + 3 * w) * m_dl[2] - 3 * m_dl[1] / m_Lr);
				//stiffness
				m_Kl[0 + 3 * 0] = E * A / m_Lr;
				m_Kl[2 + 3 * 2] = 4 * m * (1 + 3 * w) * E * I33 / m_Lr;
				m_Kl[1 + 3 * 1] = 12 * m * E * I33 / m_Lr / m_Lr / m_Lr;
				m_Kl[2 + 3 * 1] = m_Kl[1 + 3 * 2] = -6 * m * E * I33 / m_Lr / m_Lr;
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
					B[1 + 2 * 2] = 2 * (3 * a - 1) / m_Lr;
					B[1 + 2 * 1] = 6 * (1 - 2 * a) / m_Lr / m_Lr;
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
//std
#include <stdexcept>

//Math
#include "Math/inc/Groups/ASO3.hpp"
#include "Math/inc/Groups/GSO3.hpp"

//Materials
#include "Materials/inc/Mechanic/Stress.hpp"
#include "Materials/inc/Mechanic/Uniaxial.hpp"
#include "Materials/inc/Mechanic/Multiaxial.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Analysis/Assembler/Compute.hpp"
#include "FEA/inc/Analysis/Assembler/Request.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam3D.hpp"

//static
static const fea::mesh::nodes::DOF du1 = fea::mesh::nodes::DOF::Translation_1;
static const fea::mesh::nodes::DOF du2 = fea::mesh::nodes::DOF::Translation_2;
static const fea::mesh::nodes::DOF du3 = fea::mesh::nodes::DOF::Translation_3;

//right jacobian gradient: d(J(v) a) / dv
static math::Mat3 jacobian_gradient(const math::Vec3& v, const math::Vec3& a)
{
	const math::Vec3 w = -v;
	return -math::ASO3(w.data()).jacobian_gradient(a);
}

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam3D::Beam3D(void) : m_major_axis{0, 0, 1}
			{
				m_quadrature.order(1);
			}

			//data
			math::Vec3& Beam3D::major_axis(void)
			{
				return m_major_axis;
			}
			const math::Vec3& Beam3D::major_axis(void) const
			{
				return m_major_axis;
			}

			uint32_t Beam3D::stress_set(void) const
			{
				return m_shear ?
					1 << uint32_t(materials::Stress::Type::s11)|
					1 << uint32_t(materials::Stress::Type::s12)|
					1 << uint32_t(materials::Stress::Type::s13):
					1 << uint32_t(materials::Stress::Type::s11);
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

			//analysis
			void Beam3D::check(void)
			{
				//data
				Beam::check();
				const math::Vec3 z1(node(0)->position());
				const math::Vec3 z2(node(1)->position());
				//check
				if((z2 - z1).unit().cross(m_major_axis).norm() < 1.00e-05)
				{
					throw std::runtime_error("Error: Beam3D element major axis is parallel to length!");
				}
				if(m_material->inelastic() && m_shear && !dynamic_cast<const materials::Multiaxial*>(m_material))
				{
					throw std::runtime_error("Error: Beam3D element with shear in inelastic analysis requires a multiaxial material!");
				}
				if(m_material->inelastic() && !m_shear && !dynamic_cast<const materials::Uniaxial*>(m_material))
				{
					throw std::runtime_error("Error: Beam3D element without shear in inelastic analysis requires a uniaxial material!");
				}
			}
			void Beam3D::setup(void)
			{
				//setup
				Beam::setup();
				const math::Vec3 z1(node(0)->position());
				const math::Vec3 z2(node(1)->position());
				//triad
				const math::Vec3 s1 = (z2 - z1) / m_Lr;
				m_major_axis = (m_major_axis - s1.inner(m_major_axis) * s1).unit();
				const math::Vec3 s3 = m_major_axis;
				const math::Vec3 s2 = s3.cross(s1);
				m_Qr = {{s1[0], s2[0], s3[0]}, {s1[1], s2[1], s3[1]}, {s1[2], s2[2], s3[2]}};
				//stiffness
				const double p = 1.00e+03;
				const double A = m_section->area();
				const double I2 = m_section->inertia(0);
				const double I3 = m_section->inertia(1);
				const double S2 = m_section->shear_area(0);
				const double S3 = m_section->shear_area(1);
				const double J = m_section->torsion_constant();
				const double G = m_material->shear_modulus();
				const double E = m_material->elastic_modulus();
				m_Ks.zeros();
				m_Ks(0, 0) = E * A;
				m_Ks(3, 3) = G * J;
				m_Ks(4, 4) = E * I2;
				m_Ks(5, 5) = E * I3;
				m_Ks(1, 1) = m_shear ? G * S2 : p * E * I3 / m_Lr / m_Lr;
				m_Ks(2, 2) = m_shear ? G * S3 : p * E * I2 / m_Lr / m_Lr;
			}
			void Beam3D::compute(analysis::Data& assembler_data)
			{
				//data
				Data data;
				const math::Vec3 z1(node(0)->position());
				const math::Vec3 z2(node(1)->position());
				const math::GSO3 q1 = node(0)->rotation_gso3();
				const math::GSO3 q2 = node(1)->rotation_gso3();
				const math::Vec3 u1(node(0)->state(du1), node(0)->state(du2), node(0)->state(du3));
				const math::Vec3 u2(node(1)->state(du1), node(1)->state(du2), node(1)->state(du3));
				//kinematics
				data.m_pr = m_Qr.transpose() * (q1.inverse() * q2).log().vector();
				const math::Vec3 ph = m_Qr * data.m_pr / 2;
				data.m_R1 = q1.matrix() * m_Qr;
				data.m_Rm = (q1 * math::ASO3(ph.data()).exp()).matrix() * m_Qr;
				data.m_d = (z2 + u2 - z1 - u1) / m_Lr;
				//strains
				const math::Vec3 g = data.m_Rm.transpose() * data.m_d;
				data.m_es = {g[0] - 1, g[1], g[2], data.m_pr[0] / m_Lr, data.m_pr[1] / m_Lr, data.m_pr[2] / m_Lr};
				//compute
				compute_B(data);
				compute_T(data);
				compute_ss(data);
				if(assembler_data.m_request->m_U)
				{
					assembler_data.m_U = m_Lr * m_Ks.bilinear(data.m_es) / 2;
				}
				if(assembler_data.m_request->m_f)
				{
					data.m_fe = m_Lr * data.m_B.transpose() * data.m_ss;
					data.m_fe.write(assembler_data.m_f);
				}
				if(assembler_data.m_request->m_K)
				{
					compute_G(data);
					data.m_Ke = m_Lr * data.m_B.transpose() * data.m_Ks * data.m_B + data.m_G;
					for(uint32_t i = 0; i < 12; i++)
					{
						data.m_Ke.span<1, 3>(i, 3) = math::Matrix<1, 3>(data.m_Ke.span<1, 3>(i, 3) * data.m_T1);
						data.m_Ke.span<1, 3>(i, 9) = math::Matrix<1, 3>(data.m_Ke.span<1, 3>(i, 9) * data.m_T2);
					}
					data.m_Ke.write(assembler_data.m_K);
				}
			}

			//compute
			void Beam3D::compute_B(Data& data) const
			{
				//data
				const math::Mat3 I(math::Mode::eye);
				const math::Vec3 pr = data.m_pr;
				const math::Vec3 ph = -pr / 2;
				const math::Vec3 pm = -pr;
				const math::Mat3 Rt = data.m_Rm.transpose();
				const math::Mat3 D = Rt * data.m_d.spin();
				//variations (spatial spins)
				data.m_Ar = math::ASO3(pm.data()).jacobian_inverse() * data.m_R1.transpose();
				data.m_Hr = data.m_R1 * math::ASO3(ph.data()).jacobian() * data.m_Ar / 2;
				//operator
				data.m_B.zeros();
				data.m_B.span<3, 3>(0, 0) = -Rt / m_Lr;
				data.m_B.span<3, 3>(0, 6) = +Rt / m_Lr;
				data.m_B.span<3, 3>(0, 3) = D * (I - data.m_Hr);
				data.m_B.span<3, 3>(0, 9) = D * data.m_Hr;
				data.m_B.span<3, 3>(3, 3) = -data.m_Ar / m_Lr;
				data.m_B.span<3, 3>(3, 9) = +data.m_Ar / m_Lr;
			}
			void Beam3D::compute_G(Data& data) const
			{
				//data
				const math::Mat3 I(math::Mode::eye);
				const math::Vec3 pr = data.m_pr;
				const math::Vec3 ph = pr / 2;
				const math::Mat3 R1 = data.m_R1;
				const math::Vec3 n(data.m_ss[0], data.m_ss[1], data.m_ss[2]);
				const math::Vec3 m(data.m_ss[3], data.m_ss[4], data.m_ss[5]);
				const math::Mat3 Jh = math::ASO3(ph.data()).jacobian();
				const math::Mat3 Ji = math::ASO3(pr.data()).jacobian_inverse();
				//forces
				const math::Vec3 F = data.m_Rm * n;
				const math::Vec3 q = m_Lr * F.cross(data.m_d);
				const math::Vec3 u = Ji * (m + Jh * R1.transpose() * q / 2);
				const math::Vec3 c = R1 * u;
				//variations
				math::Matrix<3, 12> W1(math::Mode::zeros), Wm(math::Mode::zeros), Wp(math::Mode::zeros), Dd(math::Mode::zeros);
				W1.span<3, 3>(0, 3) = I;
				Wm.span<3, 3>(0, 3) = I - data.m_Hr;
				Wm.span<3, 3>(0, 9) = data.m_Hr;
				Wp.span<3, 3>(0, 3) = -data.m_Ar;
				Wp.span<3, 3>(0, 9) = +data.m_Ar;
				Dd.span<3, 3>(0, 0) = -I / m_Lr;
				Dd.span<3, 3>(0, 6) = +I / m_Lr;
				const math::Matrix<3, 12> dF = -F.spin() * Wm;
				const math::Matrix<3, 12> dq = m_Lr * (data.m_d.spin() * F.spin() * Wm + F.spin() * Dd);
				const math::Matrix<3, 12> dp = jacobian_gradient(ph, R1.transpose() * q) * Wp / 4 + Jh * R1.transpose() * (dq + q.spin() * W1) / 2;
				const math::Matrix<3, 12> du = Ji * (dp - jacobian_gradient(pr, u) * Wp);
				const math::Matrix<3, 12> dc = R1 * du - c.spin() * W1;
				//geometric stiffness
				data.m_G.span<3, 12>(0, 0) = -dF;
				data.m_G.span<3, 12>(3, 0) = dq - dc;
				data.m_G.span<3, 12>(6, 0) = dF;
				data.m_G.span<3, 12>(9, 0) = dc;
			}
			void Beam3D::compute_T(Data& data) const
			{
				const math::Vec3 t1 = -node(0)->rotation_aso3().vector();
				const math::Vec3 t2 = -node(1)->rotation_aso3().vector();
				data.m_T1 = math::ASO3(t1.data()).jacobian();
				data.m_T2 = math::ASO3(t2.data()).jacobian();
			}
			void Beam3D::compute_ss(Data& data)
			{
				if(!m_material->inelastic())
				{
					data.m_Ks = m_Ks;
					data.m_ss = m_Ks * data.m_es;
				}
				else
				{
					data.m_ss.zeros();
					data.m_Ks.zeros();
					for(points::Fiber& fiber : m_points[0].fibers())
					{
						//data
						const double A = fiber.area();
						const double x2 = fiber.position(0);
						const double x3 = fiber.position(1);
						//kinematics
						data.m_Ap = {{1, 0, 0, 0, x3, -x2}, {0, 1, 0, -x3, 0, 0}, {0, 0, 1, x2, 0, 0}};
						data.m_ep = data.m_Ap * data.m_es;
						//material
						data.m_sp.zeros();
						data.m_Kp.zeros();
						m_material->return_mapping(data.m_sp.data(), data.m_Kp.data(), data.m_ep.data(), fiber.material_point());
						//section
						data.m_ss += A * data.m_Ap.transpose() * data.m_sp;
						data.m_Ks += A * data.m_Ap.transpose() * data.m_Kp * data.m_Ap;
					}
					if(!m_shear)
					{
						for(uint32_t i = 1; i < 4; i++)
						{
							for(uint32_t j = 0; j < 6; j++)
							{
								data.m_Ks(i, j) = data.m_Ks(j, i) = 0;
							}
							data.m_Ks(i, i) = m_Ks(i, i);
							data.m_ss[i] = m_Ks(i, i) * data.m_es[i];
						}
					}
				}
			}
		}
	}
}

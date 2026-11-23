//std
#include <stdexcept>

//Math
#include "Math/inc/Groups/ASO3.hpp"
#include "Math/inc/Groups/GSO3.hpp"
#include "Math/inc/Miscellaneous/rotations.hpp"

//Materials
#include "Materials/inc/Mechanic/Stress.hpp"
#include "Materials/inc/Mechanic/Uniaxial.hpp"
#include "Materials/inc/Mechanic/Multiaxial.hpp"

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Analysis/Assembler/Data.hpp"
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

//radial derivatives: (d / t dt)^k fn(t)
static double fn(double t, uint32_t n, uint32_t k)
{
	return k == 0 ? math::fn(t, n) : n * fn(t, n + 2, k - 1) - fn(t, n + 1, k - 1);
}

//left jacobian inverse derivatives: f(v) = a' * A(v) * b, A(v) = J(-v)^-1 = I - spin(v) / 2 + c * spin(v)^2
static void jacobian_inverse_derivatives(math::Vec3* g, math::Mat3* H, const math::Vec3& v, const math::Vec3& a, const math::Vec3& b)
{
	//radial: c = (f3 - 2 f4) / f2 / 2
	double N[3], M[3];
	const double t = v.norm();
	for(uint32_t k = 0; k < 3; k++)
	{
		M[k] = fn(t, 2, k);
		N[k] = (fn(t, 3, k) - 2 * fn(t, 4, k)) / 2;
	}
	const double c0 = N[0] / M[0];
	const double c1 = N[1] / M[0] - N[0] * M[1] / M[0] / M[0];
	const double c2 = N[2] / M[0] - 2 * N[1] * M[1] / M[0] / M[0] - N[0] * M[2] / M[0] / M[0] + 2 * N[0] * M[1] * M[1] / M[0] / M[0] / M[0];
	const double k1 = 2 * c0 + t * t * c1;
	const double k2 = 4 * c1 + t * t * c2;
	//data: f = (a' * b) * (1 - c t^2) - v' * (b x a) / 2 + c * (a' * v) * (b' * v)
	const double s = a.inner(b);
	const double q = a.inner(v) * b.inner(v);
	const math::Mat3 I(math::Mode::eye);
	const math::Vec3 w = b.cross(a);
	const math::Vec3 p = b.inner(v) * a + a.inner(v) * b;
	//gradient
	if(g) *g = (q * c1 - s * k1) * v - w / 2 + c0 * p;
	//hessian
	if(H) *H = (q * c1 - s * k1) * I + (q * c2 - s * k2) * v.outer() + c1 * (v.outer(p) + p.outer(v)) + c0 * (a.outer(b) + b.outer(a));
}
static math::Mat3 jacobian_inverse_gradient(const math::Vec3& v, const math::Vec3& a, const math::Vec3& b, bool transpose)
{
	//gradient: d(A(v) * b) / dv or d(A(v)' * a) / dv
	math::Mat3 G;
	for(uint32_t i = 0; i < 3; i++)
	{
		math::Vec3 gi;
		const math::Vec3 e(i == 0, i == 1, i == 2);
		jacobian_inverse_derivatives(&gi, nullptr, v, transpose ? a : e, transpose ? e : b);
		for(uint32_t j = 0; j < 3; j++) G(i, j) = gi[j];
	}
	return G;
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
				Beam::check();
				const math::Vec3 z1(node(0)->position());
				const math::Vec3 z2(node(1)->position());
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
				//data
				const double A = m_section->area();
				const double I2 = m_section->inertia(0);
				const double I3 = m_section->inertia(1);
				const double S2 = m_section->shear_area(0);
				const double S3 = m_section->shear_area(1);
				const double G = m_material->shear_modulus();
				const double E = m_material->elastic_modulus();
				const double J = m_section->torsion_constant();
				//setup
				Beam::setup();
				const math::Vec3 z1(node(0)->position());
				const math::Vec3 z2(node(1)->position());
				//axis
				const math::Vec3 s1 = (z2 - z1) / m_Lr;
				m_major_axis = (m_major_axis - s1.inner(m_major_axis) * s1).unit();
				//triad
				const math::Vec3 s3 = m_major_axis;
				const math::Vec3 s2 = s3.cross(s1);
				m_Qr = {{s1[0], s2[0], s3[0]}, {s1[1], s2[1], s3[1]}, {s1[2], s2[2], s3[2]}};
				//stiffness
				m_Ks.zeros();
				m_Ks(0, 0) = E * A;
				m_Ks(3, 3) = G * J;
				m_Ks(4, 4) = E * I2;
				m_Ks(5, 5) = E * I3;
				m_Ks(1, 1) = G * (S2 > 0 ? S2 : A);
				m_Ks(2, 2) = G * (S3 > 0 ? S3 : A);
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
				data.m_R1 = q1.matrix() * m_Qr;
				data.m_dx = (z2 + u2 - z1 - u1) / m_Lr;
				data.m_pr = m_Qr.transpose() * (q1.inverse() * q2).log().vector();
				data.m_A = math::ASO3(-data.m_pr).jacobian_inverse();
				//strains (helicoidal: x2 - x1 = Lr * R1 * J(-pr) * (e1 + g))
				const math::Vec3 g = data.m_A * data.m_R1.transpose() * data.m_dx;
				data.m_es = {g[0] - 1, g[1], g[2], data.m_pr[0] / m_Lr, data.m_pr[1] / m_Lr, data.m_pr[2] / m_Lr};
				//compute
				compute_B(data);
				compute_T(data);
				compute_ss(data);
				if(assembler_data.m_request->m_U)
				{
					assembler_data.m_U = m_Lr * m_Ks.bilinear(data.m_es) / 2;
				}
				if(assembler_data.m_request->m_f || assembler_data.m_request->m_K)
				{
					data.m_fe = m_Lr * data.m_B.transpose() * data.m_ss;
				}
				if(assembler_data.m_request->m_K)
				{
					compute_G(data);
					data.m_Ke = m_Lr * data.m_B.transpose() * data.m_Ks * data.m_B + data.m_G;
					//material: K = T' * K * T + d(T' * m) / dt with T = Q * J(t) and m the spatial moment
					for(uint32_t i = 0; i < 2; i++)
					{
						const uint32_t k = 6 * i + 3;
						const math::Vec3 v = data.m_Q[i].transpose() * math::Vec3(data.m_fe.data() + k);
						data.m_Ke.span<3, 12>(k, 0) = data.m_T[i].transpose() * data.m_Ke.span<3, 12>(k, 0);
						data.m_Ke.span<12, 3>(0, k) = data.m_Ke.span<12, 3>(0, k) * data.m_T[i];
						data.m_Ke.span<3, 3>(k, k) += math::ASO3(data.m_t[i]).jacobian_gradient(v);
						data.m_Ke.span<3, 3>(k, k) += data.m_J[i].transpose() * v.spin() * data.m_J[i];
					}
					data.m_Ke.write(assembler_data.m_K);
				}
				if(assembler_data.m_request->m_f)
				{
					//material: f = T' * f
					for(uint32_t i = 0; i < 2; i++)
					{
						data.m_fe.span<3, 1>(6 * i + 3, 0) = data.m_T[i].transpose() * data.m_fe.span<3, 1>(6 * i + 3, 0);
					}
					data.m_fe.write(assembler_data.m_f);
				}
			}

			//compute
			void Beam3D::compute_B(Data& data) const
			{
				//data
				const math::Mat3 Rt = data.m_R1.transpose();
				const math::Vec3 v = Rt * data.m_dx;
				//variations
				data.m_Ar = data.m_A * Rt;
				data.m_Av = jacobian_inverse_gradient(data.m_pr, v, v, false);
				//gradient
				data.m_B.zeros();
				data.m_B.span<3, 3>(0, 0) = -data.m_Ar / m_Lr;
				data.m_B.span<3, 3>(0, 6) = +data.m_Ar / m_Lr;
				data.m_B.span<3, 3>(0, 9) = +data.m_Av * data.m_Ar;
				data.m_B.span<3, 3>(3, 3) = -data.m_Ar / m_Lr;
				data.m_B.span<3, 3>(3, 9) = +data.m_Ar / m_Lr;
				data.m_B.span<3, 3>(0, 3) = data.m_A * v.spin() * Rt - data.m_Av * data.m_Ar;
			}
			void Beam3D::compute_G(Data& data) const
			{
				//data
				math::Vec3 gp;
				math::Mat3 Hp;
				const math::Mat3 R1 = data.m_R1;
				const math::Mat3 I(math::Mode::eye);
				const math::Vec3 n(data.m_ss.data() + 0);
				const math::Vec3 m(data.m_ss.data() + 3);
				const math::Vec3 dx = m_Lr * data.m_dx;
				const math::Vec3 v = R1.transpose() * dx;
				const math::Mat3 Ji = math::ASO3(data.m_pr).jacobian_inverse();
				const math::Mat3 An = jacobian_inverse_gradient(data.m_pr, n, n, true);
				jacobian_inverse_derivatives(&gp, &Hp, data.m_pr, n, v);
				//forces
				const math::Vec3 F = R1 * data.m_A.transpose() * n;
				const math::Vec3 u = Ji * (gp + m);
				const math::Vec3 c = R1 * u;
				//variations
				math::Matrix<3, 12> W1(math::Mode::zeros), Wp(math::Mode::zeros), Dd(math::Mode::zeros);
				W1.span<3, 3>(0, 3) = I;
				Wp.span<3, 3>(0, 3) = -data.m_Ar;
				Wp.span<3, 3>(0, 9) = +data.m_Ar;
				Dd.span<3, 3>(0, 0) = -I;
				Dd.span<3, 3>(0, 6) = +I;
				const math::Matrix<3, 12> dv = R1.transpose() * Dd + v.spin() * R1.transpose() * W1;
				const math::Matrix<3, 12> dF = R1 * An * Wp - F.spin() * W1;
				const math::Matrix<3, 12> dp = Hp * Wp + An.transpose() * dv;
				const math::Matrix<3, 12> du = Ji * (dp - jacobian_gradient(data.m_pr, u) * Wp);
				const math::Matrix<3, 12> dc = R1 * du - c.spin() * W1;
				//Hesssian
				data.m_G.span<3, 12>(0, 0) = -dF;
				data.m_G.span<3, 12>(3, 0) = F.spin() * Dd - dx.spin() * dF - dc;
				data.m_G.span<3, 12>(6, 0) = dF;
				data.m_G.span<3, 12>(9, 0) = dc;
			}
			void Beam3D::compute_T(Data& data) const
			{
				for(uint32_t i = 0; i < 2; i++)
				{
					data.m_t[i] = node(i)->rotation_aso3().vector();
					data.m_Q[i] = node(i)->rotation_gso3().matrix();
					data.m_J[i] = math::ASO3(data.m_t[i]).jacobian();
					data.m_T[i] = data.m_Q[i] * data.m_J[i];
				}
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
						//fiber
						const double A = fiber.area();
						const double x2 = fiber.position(0);
						const double x3 = fiber.position(1);
						data.m_Ap = {{1, 0, 0, 0, x3, -x2}, {0, 1, 0, -x3, 0, 0}, {0, 0, 1, x2, 0, 0}};
						//material
						data.m_sp.zeros();
						data.m_Kp.zeros();
						data.m_ep = data.m_Ap * data.m_es;
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

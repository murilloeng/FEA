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
				m_Rr = {{s1[0], s2[0], s3[0]}, {s1[1], s2[1], s3[1]}, {s1[2], s2[2], s3[2]}};
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
				const math::Vec3 e1(1, 0, 0);
				const math::Vec3 z1(node(0)->position());
				const math::Vec3 z2(node(1)->position());
				const math::GSO3 q1 = node(0)->rotation_gso3();
				const math::GSO3 q2 = node(1)->rotation_gso3();
				const math::Vec3 u1(node(0)->state(du1), node(0)->state(du2), node(0)->state(du3));
				const math::Vec3 u2(node(1)->state(du1), node(1)->state(du2), node(1)->state(du3));
				//kinematics
				const math::Vec3 x1 = z1 + u1;
				const math::Vec3 x2 = z2 + u2;
				data.m_R1 = q1.matrix() * m_Rr;
				data.m_tr = m_Rr.transpose() * (q1.inverse() * q2).log().vector();
				//groups
				data.m_xr = data.m_R1.transpose() * (x2 - x1);
				data.m_T1 = node(0)->rotation_aso3().jacobian();
				data.m_T2 = node(1)->rotation_aso3().jacobian();
				data.m_Ji = math::ASO3(data.m_tr).jacobian_inverse();
				data.m_Gi = math::ASO3(data.m_tr).jacobian_inverse_gradient(data.m_xr, true);
				//strains
				data.m_es.span<3, 1>(3, 0) = data.m_tr / m_Lr;
				data.m_es.span<3, 1>(0, 0) = data.m_Ji.transpose() * data.m_xr / m_Lr - e1;
				//compute
				if(assembler_data.m_request->m_U)
				{
					assembler_data.m_U = m_Lr * m_Ks.bilinear(data.m_es) / 2;
				}
				if(assembler_data.m_request->m_f || assembler_data.m_request->m_K)
				{
					compute_section(data);
					compute_strains_gradient(data);
				}
				if(assembler_data.m_request->m_K)
				{
					compute_strains_hessian(data);
					(m_Lr * (data.m_B.transpose() * data.m_Ks * data.m_B + data.m_G)).write(assembler_data.m_K);
				}
				if(assembler_data.m_request->m_f)
				{
					(m_Lr * data.m_B.transpose() * data.m_ss).write(assembler_data.m_f);
				}
			}

			//compute
			void Beam3D::compute_shear(Data& data)
			{
				for(uint32_t i = 1; i < 4; i++)
				{
					for(uint32_t j = 0; j < 6; j++)
					{
						data.m_Ks(i, j) = 0;
						data.m_Ks(j, i) = 0;
					}
					data.m_Ks(i, i) = m_Ks(i, i);
					data.m_ss[i] = m_Ks(i, i) * data.m_es[i];
				}
			}
			void Beam3D::compute_section(Data& data)
			{
				if(!materials::Mechanic::inelastic())
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
						data.m_A = {{1, 0, 0, 0, x3, -x2}, {0, 1, 0, -x3, 0, 0}, {0, 0, 1, x2, 0, 0}};
						//material
						data.m_sp.zeros();
						data.m_Kp.zeros();
						data.m_ep = data.m_A * data.m_es;
						m_material->return_mapping(data.m_sp.data(), data.m_Kp.data(), data.m_ep.data(), fiber.material_point());
						//section
						data.m_ss += A * data.m_A.transpose() * data.m_sp;
						data.m_Ks += A * data.m_A.transpose() * data.m_Kp * data.m_A;
					}
					if(!m_shear) compute_shear(data);
				}
			}
			void Beam3D::compute_strains_hessian(Data& data)
			{
				//data
				const math::Vec3& tr = data.m_tr;
				const math::Mat3& T1 = data.m_T1;
				const math::Mat3& T2 = data.m_T2;
				const math::Mat3& Ji = data.m_Ji;
				const math::Mat3& R1 = data.m_R1;
				const math::Mat3 T1t = T1.transpose();
				const math::Mat3 T2t = T2.transpose();
				const math::Mat3 Jit = Ji.transpose();
				const math::Mat3 R1t = R1.transpose();
				const math::Mat3 Rrt = m_Rr.transpose();
				const math::Vec3 fs(data.m_ss.data() + 0);
				const math::Vec3 ms(data.m_ss.data() + 3);
				const math::Mat3 Fs = math::ASO3(tr).jacobian_inverse_gradient(fs, false);
				//hessian
				data.m_G.zeros();
				data.m_G.span<3, 3>(0, 9) = -R1 * Fs * Ji * Rrt * T2 / m_Lr;
				data.m_G.span<3, 3>(6, 9) = +R1 * Fs * Ji * Rrt * T2 / m_Lr;
				data.m_G.span<3, 3>(9, 0) = -T2t * m_Rr * Jit * Fs.transpose() * R1t / m_Lr;
				data.m_G.span<3, 3>(9, 6) = +T2t * m_Rr * Jit * Fs.transpose() * R1t / m_Lr;
				data.m_G.span<3, 3>(0, 3) = +R1 * (Fs * Jit + (Ji * fs).spin()) * Rrt * T1 / m_Lr;
				data.m_G.span<3, 3>(6, 3) = -R1 * (Fs * Jit + (Ji * fs).spin()) * Rrt * T1 / m_Lr;
				data.m_G.span<3, 3>(3, 0) = +T1t * m_Rr * (Ji * Fs.transpose() - (Ji * fs).spin()) * R1t / m_Lr;
				data.m_G.span<3, 3>(3, 6) = -T1t * m_Rr * (Ji * Fs.transpose() - (Ji * fs).spin()) * R1t / m_Lr;
			}
			void Beam3D::compute_strains_gradient(Data& data)
			{
				//data
				const math::Mat3& Ji = data.m_Ji;
				const math::Mat3& Gi = data.m_Gi;
				const math::Mat3& T1 = data.m_T1;
				const math::Mat3& T2 = data.m_T2;
				const math::Mat3 Xr = data.m_xr.spin();
				const math::Mat3 Rrt = m_Rr.transpose();
				const math::Mat3 R1t = data.m_R1.transpose();
				const math::Mat3 Jit = data.m_Ji.transpose();
				//gradient
				data.m_B.zeros();
				data.m_B.span<3, 3>(0, 0) = -Jit * R1t / m_Lr;
				data.m_B.span<3, 3>(0, 6) = +Jit * R1t / m_Lr;
				data.m_B.span<3, 3>(3, 9) = +Ji * Rrt * T2 / m_Lr;
				data.m_B.span<3, 3>(3, 3) = -Jit * Rrt * T1 / m_Lr;
				data.m_B.span<3, 3>(0, 9) = +Gi * Ji * Rrt * T2 / m_Lr;
				data.m_B.span<3, 3>(0, 3) = (Jit * Xr - Gi * Jit) * Rrt * T1 / m_Lr;
			}
		}
	}
}

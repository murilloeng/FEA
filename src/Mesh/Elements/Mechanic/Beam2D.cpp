//std
#include <cmath>

//Math
#include "Math/inc/Linear/Vec2.hpp"
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Linear/Vector.hpp"
#include "Math/inc/Linear/Matrix.hpp"

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
#include "FEA/inc/Analysis/Assembler/Compute.hpp"
#include "FEA/inc/Analysis/Assembler/Request.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

//static
static const fea::mesh::nodes::DOF dt3 = fea::mesh::nodes::DOF::Rotation_3;
static const fea::mesh::nodes::DOF du1 = fea::mesh::nodes::DOF::Translation_1;
static const fea::mesh::nodes::DOF du2 = fea::mesh::nodes::DOF::Translation_2;
// static double arc_length(double x) { return fabs(x) < 1e-4 ? 1 + x * x / 6 : asin(x) / x; }

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam2D::Beam2D(void) : m_curvature{0}
			{
				m_quadrature.order(1);
			}

			//data
			double Beam2D::curvature(void) const
			{
				return m_curvature;
			}
			double Beam2D::curvature(double curvature)
			{
				return m_curvature = curvature;
			}

			bool Beam2D::large_strains(void)
			{
				return m_large_strains;
			}
			bool Beam2D::large_strains(bool large_strains)
			{
				return m_large_strains = large_strains;
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

			//analysis
			void Beam2D::setup(void)
			{
				//data
				Beam::setup();
				const double p = 1.00e+03;
				const double A = m_section->area();
				const double I = m_section->inertia(1);
				const double S = m_section->shear_area(0);
				const double G = m_material->shear_modulus();
				const double E = m_material->elastic_modulus();
				const double K = m_shear ? G * S : p * E * I / m_Lr / m_Lr;
				//setup
				m_Ks = {{E * A, 0, 0}, {0, K, 0}, {0, 0, E * I}};
				
			}
			void Beam2D::compute(analysis::Compute& compute)
			{
				//data
				math::Vec3 es, ss;
				math::Vector f(compute.m_f, 6);
				math::Matrix K(compute.m_K, 6, 6);
				const double t1 = node(0)->state(dt3);
				const double t2 = node(1)->state(dt3);
				math::Matrix L(3, 4, math::Mode::zeros);
				math::Matrix N(4, 6, math::Mode::zeros);
				math::Matrix dL(4, 4, math::Mode::zeros);
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//compute
				compute.reset();
				const double q0 = (z2 - z1).direction();
				for(uint32_t k = 0; k < m_quadrature.order(); k++)
				{
					//point
					const double sk = m_quadrature.point(k);
					const double wk = m_quadrature.weight(k);
					//data
					const double dN1 = -1 / m_Lr;
					const double dN2 = +1 / m_Lr;
					const double N1 = (1 - sk) / 2;
					const double N2 = (1 + sk) / 2;
					const double qn = q0 + N1 * t1 + N2 * t2;
					const math::Vec2 a0 = {cos(q0), sin(q0)};
					const math::Vec2 an = {cos(qn), sin(qn)};
					const math::Vec2 bn = {-sin(qn), cos(qn)};
					//strains
					es[2] = dN1 * t1 + dN2 * t2;
					es[1] = (a0 + dN1 * u1 + dN2 * u2).inner(bn);
					es[0] = (a0 + dN1 * u1 + dN2 * u2).inner(an) - 1;
					//kinematics
					L(2, 3) = 1;
					L(0, 0) = an[0];
					L(0, 1) = an[1];
					L(1, 0) = bn[0];
					L(1, 1) = bn[1];
					L(0, 2) = es[1];
					L(1, 2) = -(1 + es[0]);
					//kinematics
					ss = m_Ks * es;
					dL(2, 2) = -(1 + es[0]) * ss[0] - es[1] * ss[1];
					dL(0, 2) = dL(2, 0) = ss[0] * bn[0] - ss[1] * an[0];
					dL(1, 2) = dL(2, 1) = ss[0] * bn[1] - ss[1] * an[1];
					//interpolation
					N(2, 2) = N1;
					N(2, 5) = N2;
					N(0, 0) = N(1, 1) = N(3, 2) = dN1;
					N(0, 3) = N(1, 4) = N(3, 5) = dN2;
					//compute
					if(compute.m_request->m_U) compute.m_U += wk * m_Lr / 2 * m_Ks.bilinear(es, es) / 2;
					if(compute.m_request->m_fi) f += wk * m_Lr / 2 * N.transpose() * L.transpose() * ss;
					if(compute.m_request->m_K) K += wk * m_Lr / 2 * N.transpose() * (L.transpose() * m_Ks * L + dL) * N;
				}
			}

			void Beam2D::internal_force(double* ptr) const
			{
				// //data
				// math::Vec3 es, ss;
				// math::Vector f(ptr, 6);
				// const double t1 = node(0)->state(dt3);
				// const double t2 = node(1)->state(dt3);
				// const math::Vec2 z1 = node(0)->position();
				// const math::Vec2 z2 = node(1)->position();
				// const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				// const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				// //compute
				// const double q0 = (z2 - z1).direction();
				// for(uint32_t k = 0; k < m_quadrature.order(); k++)
				// {
				// 	//point
				// 	const double sk = m_quadrature.point(k);
				// 	const double wk = m_quadrature.weight(k);
				// 	//data
				// 	const double dN1 = -1 / m_Lr;
				// 	const double dN2 = +1 / m_Lr;
				// 	const double N1 = (1 - sk) / 2;
				// 	const double N2 = (1 + sk) / 2;
				// 	const double qn = q0 + N1 * t1 + N2 * t2;
				// 	const math::Vec2 a0 = {cos(q0), sin(q0)};
				// 	const math::Vec2 an = {cos(qn), sin(qn)};
				// 	const math::Vec2 bn = {-sin(qn), cos(qn)};
				// 	//strains
				// 	es[2] = dN1 * t1 + dN2 * t2;
				// 	es[1] = (a0 + dN1 * u1 + dN2 * u2).inner(bn);
				// 	es[0] = (a0 + dN1 * u1 + dN2 * u2).inner(an) - 1;
				// 	//compute
				// 	// f += wk * m_Lr / 2 * N.transpose() * L.transpose() * ss;
				// }
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
			bool Beam2D::m_large_strains = false;
		}
	}
}
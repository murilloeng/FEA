//std
#include <cmath>
#include <cstring>

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
				const double* z1 = node(0)->position();
				const double* z2 = node(1)->position();
				m_q0 = atan2(z2[1] - z1[1], z2[0] - z1[0]);
				m_a0 = {cos(m_q0), sin(m_q0)};
				m_Ks = {{E * A, 0, 0}, {0, K, 0}, {0, 0, E * I}};
			}
			void Beam2D::compute(analysis::Compute& compute)
			{
				//data
				math::MatrixFixed<3, 4> L;
				math::MatrixFixed<4, 6> N;
				math::MatrixFixed<4, 4> dL;
				math::VectorFixed<3> es, ss;
				const double t1 = node(0)->state(dt3);
				const double t2 = node(1)->state(dt3);
				const double qn = m_q0 + (t1 + t2) / 2;
				const math::VectorFixed<2> u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::VectorFixed<2> u2 = {node(1)->state(du1), node(1)->state(du2)};
				//compute
				const double cn = cos(qn), sn = sin(qn);
				const math::VectorFixed<2> an = {cn, sn};
				const math::VectorFixed<2> bn = {-sn, cn};
				const math::VectorFixed<2> xp = m_a0 + (u2 - u1) / m_Lr;
				//strains
				es[1] = xp.inner(bn);
				es[0] = xp.inner(an) - 1;
				es[2] = (t2 - t1) / m_Lr;
				//stresses
				ss = m_Ks * es;
				//compute
				compute_N(N);
				compute_L(L, an, bn, es);
				if(compute.m_request->m_U)
				{
					compute.m_U = m_Lr * m_Ks.bilinear(es) / 2;
				}
				if(compute.m_request->m_fi)
				{
					const math::MatrixFixed<6, 1> f = m_Lr * N.transpose() * L.transpose() * ss;
					memcpy(compute.m_f, f.data(), 6 * sizeof(double));
				}
				if(compute.m_request->m_K)
				{
					compute_dL(dL, an, bn, es, ss);
					const math::MatrixFixed<6, 6> K = m_Lr * N.transpose() * (L.transpose() * m_Ks * L + dL) * N;
					memcpy(compute.m_K, K.data(), 36 * sizeof(double));
				}
			}

			void Beam2D::compute_N(math::MatrixFixed<4, 6>& N) const
			{
				N.zeros();
				N(2, 2) = 1.0 / 2;
				N(2, 5) = 1.0 / 2;
				N(0, 0) = N(1, 1) = N(3, 2) = -1 / m_Lr;
				N(0, 3) = N(1, 4) = N(3, 5) = +1 / m_Lr;
			}
			void Beam2D::compute_L(math::MatrixFixed<3, 4>& L, const vec2& an, const vec2& bn, const vec3& es) const
			{
				L.zeros();
				L(2, 3) = 1;
				L(0, 0) = an[0];
				L(0, 1) = an[1];
				L(1, 0) = bn[0];
				L(1, 1) = bn[1];
				L(0, 2) = es[1];
				L(1, 2) = -(1 + es[0]);
			}
			void Beam2D::compute_dL(math::MatrixFixed<4, 4>& dL, const vec2& an, const vec2& bn, const vec3& es, const vec3& ss) const
			{
				dL.zeros();
				dL(2, 2) = -(1 + es[0]) * ss[0] - es[1] * ss[1];
				dL(0, 2) = dL(2, 0) = ss[0] * bn[0] - ss[1] * an[0];
				dL(1, 2) = dL(2, 1) = ss[0] * bn[1] - ss[1] * an[1];
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
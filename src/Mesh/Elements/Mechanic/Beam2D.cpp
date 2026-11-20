//std
#include <cmath>
#include <stdexcept>

//Canvas
#include "Canvas/inc/Math/vec2.hpp"
#include "Canvas/inc/Math/mat2.hpp"
#include "Canvas/inc/Vertices/Model3D.hpp"

//Materials
#include "Materials/inc/Mechanic/Stress.hpp"
#include "Materials/inc/Mechanic/Uniaxial.hpp"
#include "Materials/inc/Mechanic/Multiaxial.hpp"

//FEA
#include "FEA/inc/Draw/Data.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Analysis/Assembler/Data.hpp"
#include "FEA/inc/Analysis/Assembler/Request.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2D.hpp"

//static
static const fea::mesh::nodes::DOF dt3 = fea::mesh::nodes::DOF::Rotation_3;
static const fea::mesh::nodes::DOF du1 = fea::mesh::nodes::DOF::Translation_1;
static const fea::mesh::nodes::DOF du2 = fea::mesh::nodes::DOF::Translation_2;

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam2D::Beam2D(void)
			{
				m_quadrature.order(1);
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
			void Beam2D::check(void)
			{
				Beam::check();
				if(m_material->inelastic() && m_shear && !dynamic_cast<const materials::Multiaxial*>(m_material))
				{
					throw std::runtime_error("Error: Beam2D element with shear in inelastic analysis requires a multiaxial material!");
				}
				if(m_material->inelastic() && !m_shear && !dynamic_cast<const materials::Uniaxial*>(m_material))
				{
					throw std::runtime_error("Error: Beam2D element without shear in inelastic analysis requires a uniaxial material!");
				}
			}
			void Beam2D::setup(void)
			{
				//setup
				Beam::setup();
				const double* z1 = node(0)->position();
				const double* z2 = node(1)->position();
				m_qr = atan2(z2[1] - z1[1], z2[0] - z1[0]);
				//stiffness
				const double p = 1.00e+03;
				const double A = m_section->area();
				const double I = m_section->inertia(1);
				const double S = m_section->shear_area(0);
				const double G = m_material->shear_modulus();
				const double E = m_material->elastic_modulus();
				m_Ks = {{E * A, 0, 0}, {0, m_shear ? G * S : p * E * I / m_Lr / m_Lr, 0}, {0, 0, E * I}};
			}
			void Beam2D::compute(analysis::Data& assembler_data)
			{
				//data
				Data data;
				const double t1 = node(0)->state(dt3);
				const double t2 = node(1)->state(dt3);
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//compute
				data.m_a0 = {cos(m_qr), sin(m_qr)};
				const double qn = m_qr + (t1 + t2) / 2;
				const double cn = cos(qn), sn = sin(qn);
				const math::Vec2 dx = data.m_a0 + (u2 - u1) / m_Lr;
				//strains
				data.m_an = {cn, sn};
				data.m_bn = {-sn, cn};
				data.m_es[2] = (t2 - t1) / m_Lr;
				data.m_es[1] = dx.inner(data.m_bn);
				data.m_es[0] = dx.inner(data.m_an) - 1;
				//compute
				compute_N(data);
				compute_L(data);
				compute_ss(data);
				if(assembler_data.m_request->m_U)
				{
					assembler_data.m_U = m_Lr * m_Ks.bilinear(data.m_es) / 2;
				}
				if(assembler_data.m_request->m_f)
				{
					data.m_fe = m_Lr * data.m_N.transpose() * data.m_L.transpose() * data.m_ss;
					data.m_fe.write(assembler_data.m_f);
				}
				if(assembler_data.m_request->m_K)
				{
					compute_dL(data);
					data.m_Ke = m_Lr * data.m_N.transpose() * (data.m_L.transpose() * data.m_Ks * data.m_L + data.m_dL) * data.m_N;
					data.m_Ke.write(assembler_data.m_K);
				}
			}

			void Beam2D::compute_N(Data& data) const
			{
				data.m_N.zeros();
				data.m_N(2, 2) = 1.0 / 2;
				data.m_N(2, 5) = 1.0 / 2;
				data.m_N(0, 0) = data.m_N(1, 1) = data.m_N(3, 2) = -1 / m_Lr;
				data.m_N(0, 3) = data.m_N(1, 4) = data.m_N(3, 5) = +1 / m_Lr;
			}
			void Beam2D::compute_L(Data& data) const
			{
				data.m_L.zeros();
				data.m_L(2, 3) = 1;
				data.m_L(0, 0) = data.m_an[0];
				data.m_L(0, 1) = data.m_an[1];
				data.m_L(1, 0) = data.m_bn[0];
				data.m_L(1, 1) = data.m_bn[1];
				data.m_L(0, 2) = data.m_es[1];
				data.m_L(1, 2) = -(1 + data.m_es[0]);
			}
			void Beam2D::compute_dL(Data& data) const
			{
				data.m_dL.zeros();
				data.m_dL(2, 2) = -(1 + data.m_es[0]) * data.m_ss[0] - data.m_es[1] * data.m_ss[1];
				data.m_dL(0, 2) = data.m_dL(2, 0) = data.m_ss[0] * data.m_bn[0] - data.m_ss[1] * data.m_an[0];
				data.m_dL(1, 2) = data.m_dL(2, 1) = data.m_ss[0] * data.m_bn[1] - data.m_ss[1] * data.m_an[1];
			}
			void Beam2D::compute_ss(Data& data)
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
						data.m_Ap = {{1, 0, -x2}, {0, 1, 0}};
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
						data.m_Ks(1, 1) = m_Ks(1, 1);
						data.m_Ks(0, 1) = data.m_Ks(1, 0) = 0;
						data.m_Ks(1, 2) = data.m_Ks(2, 1) = 0;
						data.m_ss[1] = m_Ks(1, 1) * data.m_es[1];
					}
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
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
#include "FEA/inc/Mesh/Elements/Mechanic/Beam2DNew.hpp"

//static
static const fea::mesh::nodes::DOF dt3 = fea::mesh::nodes::DOF::Rotation_3;
static const fea::mesh::nodes::DOF du1 = fea::mesh::nodes::DOF::Translation_1;
static const fea::mesh::nodes::DOF du2 = fea::mesh::nodes::DOF::Translation_2;
static double arc_length(double x) { return fabs(x) < 1e-4 ? 1 + x * x / 6 : asin(x) / x; }

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Beam2DNew::Beam2DNew(void) : m_curvature{0}
			{
				return;
			}

			//destructor
			Beam2DNew::~Beam2DNew(void)
			{
				return;
			}

			//data
			double Beam2DNew::curvature(void) const
			{
				return m_curvature;
			}
			double Beam2DNew::curvature(double curvature)
			{
				return m_curvature = curvature;
			}

			//data
			uint32_t Beam2DNew::stress_set(void) const
			{
				return m_shear ?
					1 << uint32_t(materials::Stress::Type::s11)|
					1 << uint32_t(materials::Stress::Type::s12):
					1 << uint32_t(materials::Stress::Type::s11);
			}
			uint32_t Beam2DNew::dof_set(uint32_t) const
			{
				return
					1 << uint32_t(nodes::DOF::Rotation_3)|
					1 << uint32_t(nodes::DOF::Translation_1)|
					1 << uint32_t(nodes::DOF::Translation_2);
			}

			//tangents
			void Beam2DNew::inertia(double* M) const
			{
				return;
			}

			void Beam2DNew::damping(double* C) const
			{
				return;
			}

			void Beam2DNew::stiffness(double* K) const
			{
				return;
			}

			//forces
			void Beam2DNew::internal_force(double* f) const
			{
				return;
			}

			//energies
			double Beam2DNew::kinetic_energy(void) const
			{
				return 0;
			}
			double Beam2DNew::internal_energy(void) const
			{
				//data
				const double k0 = m_curvature;
				const double t1 = node(0)->state(dt3);
				const double t2 = node(1)->state(dt3);
				const math::Vec2 z1 = node(0)->position();
				const math::Vec2 z2 = node(1)->position();
				const double G = m_material->shear_modulus();
				const double E = m_material->elastic_modulus();
				const math::Vec2 u1 = {node(0)->state(du1), node(0)->state(du2)};
				const math::Vec2 u2 = {node(1)->state(du1), node(1)->state(du2)};
				//energy
				double U = 0;
				const double qm = (z2 - z1).direction();
				const double L0 = m_Lr * arc_length(k0 * m_Lr / 2);
				for(uint32_t k = 0; k < m_quadrature.order(); k++)
				{
					//point
					const double sk = m_quadrature.point(k);
					const double wk = m_quadrature.weight(k);
					//data
					const double dN1 = -1 / L0;
					const double dN2 = +1 / L0;
					const double N1 = (1 - sk) / 2;
					const double N2 = (1 + sk) / 2;
					const double q0 = qm + k0 * L0 * sk / 2;
					const double qn = q0 + N1 * t1 + N2 * t2;
					const math::Vec2 a0 = {cos(q0), sin(q0)};
					const math::Vec2 an = {cos(qn), sin(qn)};
					const math::Vec2 bn = {-sin(qn), cos(qn)};
					//strains
					const double ks = dN1 * t1 + dN2 * t2;
					const double gs = (a0 + dN1 * u1 + dN2 * u2).inner(bn);
					const double es = (a0 + dN1 * u1 + dN2 * u2).inner(an) - 1;
					//fibers
					for(const sections::fibers::Fiber* fiber : m_section->fibers())
					{
						const double s2 = fiber->position(0);
						const double J0 = 1 - s2 * k0;
						const double st = J0 + es - s2 * ks;
						const double E12 = gs / 2;
						const double E11 = (st * st + gs * gs - J0 * J0) / 2;
						U += wk * L0 / 2 * fiber->area() * (E / J0 / J0 / J0 / 2 * E11 * E11 + 2 * G / J0 * E12 * E12);
					}
				}
				//return
				return U;
			}

			//analysis
			void Beam2DNew::compute(void)
			{
				return;
			}

			//draw
			void Beam2DNew::draw_setup(draw::Data& data) const
			{
				data.m_counter_edges += 2 * m_draw_mesh;
				data.m_counter_vertices += m_draw_mesh + 1;
			}
			void Beam2DNew::draw_update(draw::Data& data) const
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
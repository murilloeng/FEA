//Math
#include "Math/inc/Groups/ASO3.hpp"
#include "Math/inc/Groups/GSO3.hpp"
#include "Math/inc/Linear/Matrix.hpp"
#include "Math/inc/Miscellaneous/rotations.hpp"

//Canvas
#include "Canvas/inc/Math/vec3.hpp"
#include "Canvas/inc/Vertices/Model3D.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Data.hpp"
#include "FEA/inc/Draw/Colors.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Joints/Rigid3D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

//radial derivatives: (d / t dt)^k fn(t)
static double fn(double t, uint32_t n, uint32_t k)
{
	return k == 0 ? math::fn(t, n) : n * fn(t, n + 2, k - 1) - fn(t, n + 1, k - 1);
}

//derivatives of p' * exp(t) * a with exp(t) = f0 * I + f1 * spin(t) + f2 * t * t'
static void exp_derivatives(double* T, math::Mat3* H, math::Vec3* g, const math::Vec3& t, const math::Vec3& p, const math::Vec3& a)
{
	//data
	double fr[3][4];
	const double tn = t.norm();
	const double s = p.inner(a);
	const math::Vec3 c = a.cross(p);
	const math::Mat3 I(math::Mode::eye);
	const double q = p.inner(t) * a.inner(t);
	const math::Mat3 M = (p.outer(a) + a.outer(p)) / 2;
	const math::Vec3 m = M * t;
	const double ct = c.inner(t);
	//radial
	for(uint32_t n = 0; n < 3; n++)
	{
		for(uint32_t k = 0; k < 4; k++)
		{
			fr[n][k] = fn(tn, n, k);
		}
	}
	const double A = s * fr[0][1] + ct * fr[1][1] + q * fr[2][1];
	const double B = s * fr[0][2] + ct * fr[1][2] + q * fr[2][2];
	const double C = s * fr[0][3] + ct * fr[1][3] + q * fr[2][3];
	//gradient
	if(g)
	{
		*g = A * t + fr[1][0] * c + 2 * fr[2][0] * m;
	}
	//hessian
	if(H)
	{
		*H = A * I + B * t.outer() + fr[1][1] * (t.outer(c) + c.outer(t)) + 2 * fr[2][1] * (t.outer(m) + m.outer(t)) + 2 * fr[2][0] * M;
	}
	//third
	if(T)
	{
		const math::Vec3 u = fr[1][2] * c + 2 * fr[2][2] * m;
		const math::Vec3 w = B * t + fr[1][1] * c + 2 * fr[2][1] * m;
		for(uint32_t i = 0; i < 3; i++)
		{
			for(uint32_t j = 0; j < 3; j++)
			{
				for(uint32_t k = 0; k < 3; k++)
				{
					T[i + 3 * j + 9 * k] = 
						w[i] * I(j, k) + w[j] * I(i, k) + w[k] * I(i, j) + C * t[i] * t[j] * t[k] + 
						u[i] * t[j] * t[k] + t[i] * u[j] * t[k] + t[i] * t[j] * u[k] + 
						2 * fr[2][1] * (t[i] * M(j, k) + t[j] * M(i, k) + t[k] * M(i, j));
				}
			}
		}
	}
}

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			//constructors
			Rigid3D::Rigid3D(void)
			{
				return;
			}

			//destructor
			Rigid3D::~Rigid3D(void)
			{
				return;
			}

			//analysis
			void Rigid3D::create_constraints(void) const
			{
				//data
				const nodes::Node* node = m_mesh->node(m_nodes[0]);
				const math::Vec3 z0(m_mesh->node(m_nodes[0])->position());
				const math::Vec3 z1(m_mesh->node(m_nodes[1])->position());
				const uint32_t nc = m_mesh->model()->boundary()->constraints().size();
				const nodes::DOF dr[] = {nodes::DOF::Rotation_1, nodes::DOF::Rotation_2, nodes::DOF::Rotation_3};
				const nodes::DOF dt[] = {nodes::DOF::Translation_1, nodes::DOF::Translation_2, nodes::DOF::Translation_3};
				//rotation
				const math::Vec3 dz = z1 - z0;
				const std::function<math::Vec3(const double*)> rotate = [node, dz](const double* t){
					return node->rotation_old().matrix() * math::ASO3(t).exp().matrix() * dz;
				};
				//constraints
				for(uint32_t i = 0; i < 3; i++)
				{
					//constraint
					m_mesh->model()->boundary()->create_constraint(
						{m_nodes[1], m_nodes[0], m_nodes[0], m_nodes[0], m_nodes[0]},
						{dt[i], dt[i], dr[0], dr[1], dr[2]}
					);
					//function
					m_mesh->model()->boundary()->constraint(nc + i)->function([i, dz, rotate](double& f, const double* x){
						f = x[0] - x[1] + dz[i] - rotate(x + 2)[i];
					});
					//gradient
					m_mesh->model()->boundary()->constraint(nc + i)->gradient([i, dz, node](double* g, const double* x){
						//data
						math::Vec3 gr;
						math::Vector<5> gc(math::Mode::zeros);
						const math::Vec3 t(x + 2), e(i == 0, i == 1, i == 2);
						//gradient
						exp_derivatives(nullptr, nullptr, &gr, t, node->rotation_old().matrix().transpose() * e, dz);
						gc[0] = +1;
						gc[1] = -1;
						gc.span<3, 1>(2, 0) = -gr;
						//write
						gc.write(g);
					});
					//hessian
					m_mesh->model()->boundary()->constraint(nc + i)->hessian([i, dz, node](double* H, const double* x){
						//data
						math::Mat3 Hr;
						math::Matrix<5, 5> Hc(math::Mode::zeros);
						const math::Vec3 t(x + 2), e(i == 0, i == 1, i == 2);
						//hessian
						exp_derivatives(nullptr, &Hr, nullptr, t, node->rotation_old().matrix().transpose() * e, dz);
						Hc.span<3, 3>(2, 2) = -Hr;
						//write
						Hc.write(H);
					});
					//third
					m_mesh->model()->boundary()->constraint(nc + i)->third([i, dz, node](double* T, const double* x){
						//data
						double Tr[27];
						math::Matrix<25, 5> Tc(math::Mode::zeros);
						const math::Vec3 t(x + 2), e(i == 0, i == 1, i == 2);
						//third
						exp_derivatives(Tr, nullptr, nullptr, t, node->rotation_old().matrix().transpose() * e, dz);
						for(uint32_t j = 0; j < 3; j++)
						{
							for(uint32_t k = 0; k < 3; k++)
							{
								for(uint32_t l = 0; l < 3; l++)
								{
									Tc(5 * (2 + j) + 2 + k, 2 + l) = -Tr[j + 3 * k + 9 * l];
								}
							}
						}
						//write
						Tc.write(T);
					});
				}
			}
			void Rigid3D::create_dependencies(void) const
			{
				m_mesh->model()->boundary()->create_dependency(m_nodes[0], nodes::DOF::Rotation_1, m_nodes[1], nodes::DOF::Rotation_1);
				m_mesh->model()->boundary()->create_dependency(m_nodes[0], nodes::DOF::Rotation_2, m_nodes[1], nodes::DOF::Rotation_2);
				m_mesh->model()->boundary()->create_dependency(m_nodes[0], nodes::DOF::Rotation_3, m_nodes[1], nodes::DOF::Rotation_3);
			}

			//draw
			void Rigid3D::draw_setup(draw::Data& data) const
			{
				data.m_counter_edges += 2;
				data.m_counter_vertices += 2;
			}
			void Rigid3D::draw_update(draw::Data& data) const
			{
				//data
				const uint32_t nn = m_mesh->nodes().size();
				uint32_t* ibo_ptr = data.m_ibo.data() + data.m_counter_dots + data.m_index_edges;
				const canvas::vec3 x1 = data.m_positions_data + 3 * nn * data.m_step + 3 * m_nodes[0];
				const canvas::vec3 x2 = data.m_positions_data + 3 * nn * data.m_step + 3 * m_nodes[1];
				canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) data.m_vbo.data() + data.m_index_vertices;
				//ibo data
				ibo_ptr[0] = data.m_index_vertices + 0;
				ibo_ptr[1] = data.m_index_vertices + 1;
				//vbo data
				vbo_ptr[0].m_position = x1;
				vbo_ptr[1].m_position = x2;
				vbo_ptr[0].m_color = vbo_ptr[1].m_color = data.m_colors.joints();
				//update
				data.m_index_edges += 2;
				data.m_index_vertices += 2;
			}
		}
	}
}
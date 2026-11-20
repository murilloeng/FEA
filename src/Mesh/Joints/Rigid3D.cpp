//Math
#include "Math/inc/Groups/ASO3.hpp"
#include "Math/inc/Groups/GSO3.hpp"
#include "Math/inc/Linear/Matrix.hpp"

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
					return math::ASO3(t).exp().matrix() * node->rotation_old().matrix() * dz;
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
					m_mesh->model()->boundary()->constraint(nc + i)->gradient([i, rotate](double* g, const double* x){
						//data
						math::Vector<5> gc(math::Mode::zeros);
						const math::Vec3 r = rotate(x + 2), e(i == 0, i == 1, i == 2);
						//gradient
						gc[0] = +1;
						gc[1] = -1;
						gc.span<3, 1>(2, 0) = e.cross(r);
						//write
						gc.write(g);
					});
					//hessian
					m_mesh->model()->boundary()->constraint(nc + i)->hessian([i, rotate](double* H, const double* x){
						//data
						math::Matrix<5, 5> Hc(math::Mode::zeros);
						const math::Vec3 r = rotate(x + 2), t(x + 2), e(i == 0, i == 1, i == 2);
						//hessian
						Hc.span<3, 3>(2, 2) = -e.spin() * r.spin() * math::ASO3(-t).jacobian();
						//write
						Hc.write(H);
					});
					//third
					m_mesh->model()->boundary()->constraint(nc + i)->third([i, rotate](double* T, const double* x){
						//data
						math::Matrix<25, 5> Tc(math::Mode::zeros);
						const math::Vec3 r = rotate(x + 2), t(x + 2), e(i == 0, i == 1, i == 2);
						const math::Mat3 J = math::ASO3(-t).jacobian();
						//third
						for(uint32_t j = 0; j < 3; j++)
						{
							const math::Vec3 u(j == 0, j == 1, j == 2), w = J * u;
							Tc.span<3, 3>(5 * (2 + j) + 2, 2) = -e.spin() * (w.spin() * r.spin() * J + r.spin() * math::ASO3(t).jacobian_gradient(u));
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
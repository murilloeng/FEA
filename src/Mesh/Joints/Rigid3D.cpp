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
					return math::Vec3(math::ASO3(t).exp().matrix() * node->rotation_old().matrix() * dz);
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
						math::Vector<5> gc(math::Mode::zeros);
						const math::Vec3 r = rotate(x + 2);
						const math::Vec3 e(i == 0, i == 1, i == 2);
						gc[0] = +1;
						gc[1] = -1;
						gc.span<3, 1>(2, 0) = e.cross(r);
						gc.write(g);
					});
					//hessian
					m_mesh->model()->boundary()->constraint(nc + i)->hessian([i, rotate](double* H, const double* x){
						math::Matrix<5, 5> Hc(math::Mode::zeros);
						const math::Vec3 r = rotate(x + 2);
						const math::Vec3 t(x + 2), e(i == 0, i == 1, i == 2);
						const math::Mat3 T = math::ASO3(-t).jacobian();
						Hc.span<3, 3>(2, 2) = -e.spin() * r.spin() * T;
						Hc.write(H);
					});
				}
				//rotation
				for(uint32_t i = 0; i < 3; i++)
				{
					//constraint
					m_mesh->model()->boundary()->create_constraint({m_nodes[1], m_nodes[0]}, {dr[i], dr[i]});
					//function
					m_mesh->model()->boundary()->constraint(nc + 3 + i)->function([](double& f, const double* x){
						f = x[0] - x[1];
					});
					//gradient
					m_mesh->model()->boundary()->constraint(nc + 3 + i)->gradient([](double* g, const double*){
						g[0] = +1;
						g[1] = -1;
					});
					//hessian
					m_mesh->model()->boundary()->constraint(nc + 3 + i)->hessian([](double* H, const double*){
						H[0] = H[1] = H[2] = H[3] = 0;
					});
				}
			}
			void Rigid3D::create_dependencies(void) const
			{
				return;
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
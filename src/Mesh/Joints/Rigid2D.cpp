//std
#include <cmath>

//Canvas
#include "Canvas/inc/Math/vec2.hpp"
#include "Canvas/inc/Vertices/Model3D.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Data.hpp"
#include "FEA/inc/Draw/What.hpp"
#include "FEA/inc/Draw/Colors.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Joints/Rigid2D.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			//constructors
			Rigid2D::Rigid2D(void)
			{
				return;
			}

			//destructor
			Rigid2D::~Rigid2D(void)
			{
				return;
			}

			//analysis
			void Rigid2D::create_constraints(void) const
			{
				//data
				const double* z0 = m_mesh->node(m_nodes[0])->position_ref();
				const double* z1 = m_mesh->node(m_nodes[1])->position_ref();
				const uint32_t nc = m_mesh->model()->boundary()->constraints().size();
				//constraints
				m_mesh->model()->boundary()->create_constraint(
					{m_nodes[1], m_nodes[0], m_nodes[0]}, 
					{nodes::DOF::Translation_1, nodes::DOF::Translation_1, nodes::DOF::Rotation_3}
				);
				m_mesh->model()->boundary()->create_constraint(
					{m_nodes[1], m_nodes[0], m_nodes[0]}, 
					{nodes::DOF::Translation_2, nodes::DOF::Translation_2, nodes::DOF::Rotation_3}
				);
				//functions
				const double dz[] = {z1[0] - z0[0], z1[1] - z0[1]};
				m_mesh->model()->boundary()->constraint(nc + 0)->function([dz](double& f, const double* x){
					f = x[0] - x[1] + (1 - cos(x[2])) * dz[0] + sin(x[2]) * dz[1];
				});
				m_mesh->model()->boundary()->constraint(nc + 1)->function([dz](double& f, const double* x){
					f = x[0] - x[1] + (1 - cos(x[2])) * dz[1] - sin(x[2]) * dz[0];
				});
				//gradients
				m_mesh->model()->boundary()->constraint(nc + 0)->gradient([dz](double* g, const double* x){
					g[0] = +1;
					g[1] = -1;
					g[2] = sin(x[2]) * dz[0] + cos(x[2]) * dz[1];
				});
				m_mesh->model()->boundary()->constraint(nc + 1)->gradient([dz](double* g, const double* x){
					g[0] = +1;
					g[1] = -1;
					g[2] = sin(x[2]) * dz[1] - cos(x[2]) * dz[0];
				});
				//hessians
				m_mesh->model()->boundary()->constraint(nc + 0)->hessian([dz](double* H, const double* x){
					H[0 + 3 * 2] = H[1 + 3 * 2] = 0;
					H[0 + 3 * 0] = H[1 + 3 * 0] = H[2 + 3 * 0] = 0;
					H[0 + 3 * 1] = H[1 + 3 * 1] = H[2 + 3 * 1] = 0;
					H[2 + 3 * 2] = cos(x[2]) * dz[0] - sin(x[2]) * dz[1];
				});
				m_mesh->model()->boundary()->constraint(nc + 1)->hessian([dz](double* H, const double* x){
					H[0 + 3 * 2] = H[1 + 3 * 2] = 0;
					H[0 + 3 * 0] = H[1 + 3 * 0] = H[2 + 3 * 0] = 0;
					H[0 + 3 * 1] = H[1 + 3 * 1] = H[2 + 3 * 1] = 0;
					H[2 + 3 * 2] = cos(x[2]) * dz[1] + sin(x[2]) * dz[0];
				});
			}
			void Rigid2D::create_dependencies(void) const
			{
				m_mesh->model()->boundary()->create_dependency(m_nodes[0], nodes::DOF::Rotation_3, m_nodes[1], nodes::DOF::Rotation_3);
			}

			//draw
			void Rigid2D::draw_setup(draw::Data& data) const
			{
				data.m_counter_edges += 2;
				data.m_counter_vertices += 2;
			}
			void Rigid2D::draw_update(draw::Data& data) const
			{
				//data
				const uint32_t nn = m_mesh->nodes().size();
				uint32_t* ibo_ptr = data.m_ibo.data() + data.m_counter_dots + data.m_index_edges;
				const canvas::vec2 x1 = data.m_positions_data + 3 * nn * data.m_step + 3 * m_nodes[0];
				const canvas::vec2 x2 = data.m_positions_data + 3 * nn * data.m_step + 3 * m_nodes[1];
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
//std
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vec3.hpp"

//Canvas
#include "Canvas/inc/Vertices/Model3D.hpp"

//FEA
#include "FEA/inc/Draw/Data.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Frame.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Frame::Frame(void) : m_section{nullptr}
			{
				return;
			}

			//destructor
			Frame::~Frame(void)
			{
				return;
			}

			//data
			const double* Frame::major_axis(void) const
			{
				return m_major_axis;
			}
			const double* Frame::major_axis(const double* major_axis)
			{
				return (const double*) memcpy(m_major_axis, major_axis, 3 * sizeof(double));
			}
			const double* Frame::major_axis(double s1, double s2, double s3)
			{
				m_major_axis[0] = s1;
				m_major_axis[1] = s2;
				m_major_axis[2] = s3;
				return m_major_axis;
			}

			const sections::Section* Frame::section(void) const
			{
				return m_section;
			}
			const sections::Section* Frame::section(sections::Section* section)
			{
				return m_section = section;
			}

			//analysis
			void Frame::check(void)
			{
				if(!m_section)
				{
					throw std::runtime_error("Error: Frame element has unset section!");
				}
				Mechanic::check();
			}
			void Frame::setup(void)
			{
				//data
				Mechanic::setup();
				const math::Vec3 z1 = node(0)->position_ref();
				const math::Vec3 z2 = node(1)->position_ref();
				//length
				m_Lr = (z2 - z1).norm();
			}

			//draw
			void Frame::draw_setup(draw::Data& data) const
			{
				data.m_counter_edges += 2;
				data.m_counter_vertices += 2;
			}
			void Frame::draw_update(draw::Data& data) const
			{
				//data
				const uint32_t nn = m_mesh->nodes().size();
				uint32_t* ibo_ptr = data.m_ibo.data() + data.m_counter_dots + data.m_index_edges;
				canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) data.m_vbo.data() + data.m_index_vertices;
				//ibo data
				ibo_ptr[0] = data.m_index_vertices + 0;
				ibo_ptr[1] = data.m_index_vertices + 1;
				//vbo data
				vbo_ptr[0].m_color = vbo_ptr[1].m_color = data.m_colors.elements();
				vbo_ptr[0].m_position = data.m_positions_data + 3 * nn * data.m_step + 3 * m_nodes[0];
				vbo_ptr[1].m_position = data.m_positions_data + 3 * nn * data.m_step + 3 * m_nodes[1];
				//update
				data.m_index_edges += 2;
				data.m_index_vertices += 2;
			}
		}
	}
}
//std
#include <cstdlib>
#include <climits>
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Time/Time.hpp"
#include "FEA/inc/Boundary/Loads/Load.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructors
			Load::Load(LoadCase* load_case) : 
				m_value{1}, m_time{UINT_MAX}, m_draw_direction{true}, m_load_case{load_case}
			{
				return;
			}

			//destructor
			Load::~Load(void)
			{
				return;
			}

			//serialization
			void Load::load(FILE* file)
			{
				if(fscanf(file, "%lf %d", &m_value, &m_time) != 2)
				{
					throw std::runtime_error("Load loading error!");
				}
			}
			void Load::save(FILE* file) const
			{
				fprintf(file, "%+.6e %02d ", m_value, m_time);
			}

			//data
			double Load::value(void) const
			{
				return m_value;
			}
			double Load::value(double value)
			{
				return m_value = value;
			}

			LoadCase* Load::load_case(void) const
			{
				return m_load_case;
			}

			time::Time* Load::time(void) const
			{
				return m_time == UINT_MAX ? nullptr : m_load_case->boundary()->time(m_time);
			}
			time::Time* Load::time(uint32_t time)
			{
				if(time != UINT_MAX && time >= m_load_case->boundary()->times().size())
				{
					throw std::runtime_error("Load's time index is out of range!");
				}
				m_time = time;
				return m_time == UINT_MAX ? nullptr : m_load_case->boundary()->time(m_time);
			}

			double Load::time_value(double t) const
			{
				return m_time == UINT_MAX ? 1 : time()->value(t);
			}

			bool Load::draw_direction(void) const
			{
				return m_draw_direction;
			}
			bool Load::draw_direction(bool draw_direction)
			{
				return m_draw_direction = draw_direction;
			}

			//index
			uint32_t Load::index_time(void) const
			{
				return m_time;
			}

			//buffers
			// void Load::buffers_size(void)
			// {
			// 	return;
			// }
			// void Load::buffers_data(void) const
			// {
			// 	return;
			// }

			//draw
			// canvas::Color Load::draw_color(uint32_t node_index) const
			// {
			// 	return m_load_case->boundary()->model()->mesh()->node(node_index)->draw_color();
			// }
			// canvas::vec3 Load::draw_position(uint32_t node_index) const
			// {
			// 	return m_load_case->boundary()->model()->mesh()->node(node_index)->draw_position();
			// }
			// canvas::quat Load::draw_rotation(uint32_t node_index) const
			// {
			// 	return m_load_case->boundary()->model()->mesh()->node(node_index)->draw_rotation();
			// }
		}
	}
}
//Galileo
#include "FEA/inc/Results/Extrema.hpp"

namespace fea
{
	namespace results
	{
		//constructor
		Extrema::Extrema(uint32_t size, const double** data) : m_size(size)
		{
			m_data[0] = data[0];
			m_data[1] = data[1];
			search(0);
			search(1);
		}

		//destructor
		Extrema::~Extrema(void)
		{
			return;
		}

		//data
		const std::vector<Extremum>& Extrema::points(uint32_t index) const
		{
			return m_points[index];
		}

		//search
		void Extrema::search(uint32_t index)
		{
			Extremum point;
			const double* data = m_data[index];
			for(uint32_t i = 0; i < m_size; i++)
			{
				if(i != 0 && data[i] * data[i - 1] <= 0)
				{
					//data
					const double v1 = data[i - 1];
					const double v2 = data[i + 0];
					const double sc = (v1 + v2) / (v1 - v2);
					//point
					if(v1 != v2 && sc > 0)
					{
						point.m_step = i;
						point.m_type = 2;
						point.m_data[0] = (1 - sc) / 2 * m_data[0][i - 1] + (1 + sc) / 2 * m_data[0][i];
						point.m_data[1] = (1 - sc) / 2 * m_data[1][i - 1] + (1 + sc) / 2 * m_data[1][i];
						m_points[index].push_back(point);
					}

				}
				else if(i + 1 != m_size && data[i] * data[i + 1] <= 0)
				{
					//data
					const double v1 = data[i + 0];
					const double v2 = data[i + 1];
					const double sc = (v1 + v2) / (v1 - v2);
					//point
					if(v1 != v2 && sc < 0)
					{
						point.m_step = i;
						point.m_type = 2;
						point.m_data[0] = (1 - sc) / 2 * m_data[0][i] + (1 + sc) / 2 * m_data[0][i + 1];
						point.m_data[1] = (1 - sc) / 2 * m_data[1][i] + (1 + sc) / 2 * m_data[1][i + 1];
						m_points[index].push_back(point);
					}
				}
				if(i != 0 && i + 1 != m_size && data[i] < data[i - 1] && data[i] < data[i + 1])
				{
					point.m_step = i;
					point.m_type = 0;
					point.m_data[0] = m_data[0][i];
					point.m_data[1] = m_data[1][i];
					m_points[index].push_back(point);
				}
				else if(i != 0 && i + 1 != m_size && data[i] > data[i - 1] && data[i] > data[i + 1])
				{
					point.m_step = i;
					point.m_type = 1;
					point.m_data[0] = m_data[0][i];
					point.m_data[1] = m_data[1][i];
					m_points[index].push_back(point);
				}
			}
		}
	}
}
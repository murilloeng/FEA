//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/Incremental.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Incremental::Incremental(void)
		{
			return;
		}
		
		//destructor
		Incremental::~Incremental(void)
		{
			return;
		}

		//draw
		uint32_t Incremental::draw_steps(void) const
		{
			return m_status ? m_step : 1;
		}
		void Incremental::draw_positions(float* positions_data, float scale) const
		{
			//data
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			const uint32_t nn = m_analysis->model()->mesh()->nodes().size();
			//positions
			for(const mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
			{
				//data
				const uint32_t index = node->index();
				const double* z = node->position_ref();
				//position
				for(uint32_t i = 0; i < 3; i++)
				{
					if(!m_status)
					{
						positions_data[3 * index + i] = float(z[i]);
					}
					else
					{
						for(uint32_t step = 0; step < m_step; step++)
						{
							positions_data[3 * nn * step + 3 * index + i] = float(z[i]);
							if(node->dof_set() & 1 << uint32_t(fea::mesh::nodes::DOF(i)))
							{
								const uint32_t dof_index = node->dof_index(fea::mesh::nodes::DOF(i));
								if(dof_index < nu) positions_data[3 * nn * step + 3 * index + i] += scale * float(m_x_data[nu * step + dof_index]);
							}
						}
					}
				}
			}
		}
		void Incremental::draw_rotations(float* rotations_data, float scale) const
		{
			//data
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			const uint32_t nn = m_analysis->model()->mesh()->nodes().size();
			//positions
			for(const mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
			{
				//data
				const uint32_t index = node->index();
				//position
				for(uint32_t i = 0; i < 3; i++)
				{
					if(!m_status)
					{
						rotations_data[3 * index + i] = 0;
					}
					else
					{
						for(uint32_t step = 0; step < m_step; step++)
						{
							rotations_data[3 * nn * step + 3 * index + i] = 0;
							if(node->dof_set() & 1 << uint32_t(fea::mesh::nodes::DOF(i + 3)))
							{
								const uint32_t dof_index = node->dof_index(fea::mesh::nodes::DOF(i + 3));
								rotations_data[3 * nn * step + 3 * index + i] = scale * float(m_x_data[nu * step + dof_index]);
							}
						}
					}
				}
			}
		}

		//save
		void Incremental::save(const char* path, std::vector<std::pair<uint32_t, mesh::nodes::DOF>> list) const
		{
			std::vector<uint32_t> dof_list;
			for(const std::pair<uint32_t, mesh::nodes::DOF>& item : list)
			{
				if(m_analysis->model()->mesh()->node(item.first)->dof_set() & 1 << uint32_t(item.second))
				{
					dof_list.push_back(m_analysis->model()->mesh()->node(item.first)->dof_index(item.second));
				}
			}
			math::solvers::Incremental::save(path, dof_list);
		}

		//analysis
		void Incremental::setup(void)
		{
			Solver::setup();
		}
	}
}
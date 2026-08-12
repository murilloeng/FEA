//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticLinear.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		StaticLinear::StaticLinear(void)
		{
			return;
		}

		//destructor
		StaticLinear::~StaticLinear(void)
		{
			return;
		}

		//solve
		void StaticLinear::solve(void)
		{
			//assemble
			model_compute();
			m_analysis->assembler()->assemble_stiffness(m_K);
			m_analysis->assembler()->assemble_reference_force(m_fe);
			//solve
			math::solvers::Solver::solve(m_K, m_fe, m_x_new);
		}

		//data
		uint32_t StaticLinear::state_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::State::x);
		}
		uint32_t StaticLinear::force_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::Force::fe);
		}
		uint32_t StaticLinear::tangent_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::Tangent::K);
		}

		//draw
		uint32_t StaticLinear::draw_steps(void) const
		{
			return m_status ? 2 : 1;
		}
		void StaticLinear::draw_positions(float* positions_data, float scale) const
		{
			//data
			Solver::draw_positions(positions_data, scale);
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			const uint32_t nn = m_analysis->model()->mesh()->nodes().size();
			//positions
			if(!m_status) return;
			for(const mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
			{
				//data
				const uint32_t index = node->index();
				const double* z = node->position_ref();
				//position
				for(uint32_t i = 0; i < 3; i++)
				{
					positions_data[3 * nn + 3 * index + i] = float(z[i]);
					if(node->dof_set() & 1 << uint32_t(fea::mesh::nodes::DOF(i)))
					{
						const uint32_t dof_index = node->dof_index(fea::mesh::nodes::DOF(i));
						positions_data[3 * nn + 3 * index + i] += 
							dof_index < nu ? scale * float(m_x_new[dof_index]) : 
							m_analysis->model()->boundary()->support(dof_index - nu)->state(m_t_new);
					}
				}
			}
		}
		void StaticLinear::draw_rotations(float* rotations_data, float scale) const
		{
			//data
			Solver::draw_positions(rotations_data, scale);
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			const uint32_t nn = m_analysis->model()->mesh()->nodes().size();
			//positions
			if(!m_status) return;
			for(const mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
			{
				//data
				const uint32_t index = node->index();
				//position
				for(uint32_t i = 0; i < 3; i++)
				{
					rotations_data[3 * nn + 3 * index + i] = 0;
					if(node->dof_set() & 1 << uint32_t(fea::mesh::nodes::DOF(i)))
					{
						const uint32_t dof_index = node->dof_index(fea::mesh::nodes::DOF(i + 3));
						rotations_data[3 * nn + 3 * index + i] += 
							dof_index < nu ? scale * float(m_x_new[dof_index]) : 
							m_analysis->model()->boundary()->support(dof_index - nu)->state(m_t_new);
					}
				}
			}
		}
	}
}
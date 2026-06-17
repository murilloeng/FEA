//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/StaticLinear.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			StaticLinear::StaticLinear(void) : m_xk{nullptr}
			{
				return;
			}

			//destructor
			StaticLinear::~StaticLinear(void)
			{
				delete[] m_xk;
			}

			//type
			Type StaticLinear::type(void) const
			{
				return Type::StaticLinear;
			}

			//sets
			uint32_t StaticLinear::state_set(void) const
			{
				return 
					(uint32_t) solvers::state::x |
					(uint32_t) solvers::state::p;
			}
			uint32_t StaticLinear::force_set(void) const
			{
				return
					(uint32_t) solvers::force::R |
					(uint32_t) solvers::force::fi |
					(uint32_t) solvers::force::fr |
					(uint32_t) solvers::force::fd ;
			}
			uint32_t StaticLinear::tangent_set(void) const
			{
				return (uint32_t) solvers::tangent::K;
			}

			//analysis
			bool StaticLinear::solve(void)
			{
				solve_setup();
				if(!solve_state()) return false;
				while(!m_stop_criteria.compute())
				{
					if(!solve_apply()) return false;
				}
				return true;
			}

			void StaticLinear::solve_setup(void)
			{
				//time
				m_t_new = m_t_max;
				m_assembler->record();
				//assembler
				m_assembler->apply();
				m_assembler->compute();
				m_assembler->assemble();
				//record
				compute_supports();
				compute_reactions();
			}
			bool StaticLinear::solve_state(void)
			{
				for(uint32_t i = 0; i < m_assembler->dof_unknow(); i++)
				{
					m_fru[i] += m_fdu[i] - m_fiu[i];
				}
				if(!linear_solve(m_dx, m_K, m_fru))
				{
					printf("Unable to decompose stiffness matrix!\n");
					return false;
				}
				return true;
			}
			bool StaticLinear::solve_apply(void)
			{
				//step
				m_step++;
				if(m_stop) return false;
				if(m_interface) m_interface();
				//apply
				compute_state();
				m_assembler->apply_state();
				//compute
				m_assembler->compute();
				//assemble
				m_assembler->assemble_internal_force();
				m_assembler->assemble_internal_energy();
				//record
				compute_reactions();
				m_assembler->record();
				//print
				printf("step: %04d load: %+.2e dof: %+.2e\n", m_step, m_p_new, m_watch_dof.value());
				//return
				return true;
			}

			//compute
			void StaticLinear::compute_state(void)
			{
				//data
				m_p_new = double(m_step) / m_step_max;
				const uint32_t nk = m_assembler->dof_know();
				const uint32_t nu = m_assembler->dof_unknow();
				//dof know
				for(uint32_t i = 0; i < nk; i++)
				{
					const uint32_t node = m_analysis->model()->boundary()->support(i)->index_node();
					const mesh::nodes::dof dof = m_analysis->model()->boundary()->support(i)->dof();
					m_analysis->model()->mesh()->node(node)->state(dof) = m_p_new * m_xk[i];
				}
				//dof unknow
				for(uint32_t i = 0; i < nu; i++)
				{
					m_x_new[i] = m_p_new * m_dx[i];
				}
			}
			void StaticLinear::compute_supports(void)
			{
				//data
				const uint32_t nk = m_assembler->dof_know();
				const uint32_t nu = m_assembler->dof_unknow();
				//state
				delete[] m_xk;
				m_xk = new double[nk];
				for(const boundary::Support* support : m_analysis->model()->boundary()->supports())
				{
					//data
					const uint32_t node = support->index_node();
					const mesh::nodes::dof dof = support->dof();
					const uint32_t dof_index = m_analysis->model()->mesh()->node(node)->dof_index(dof);
					//state
					m_xk[dof_index - nu] = support->dof_state(m_t_max);
				}
			}
		}
	}
}
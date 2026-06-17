//std
#include <cmath>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/Modal.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Modal::Modal(void)
			{
				return;
			}

			//destructor
			Modal::~Modal(void)
			{
				return;
			}

			//serialization
			void Modal::load(FILE* file)
			{
				Eigen::load(file);
			}
			void Modal::save(FILE* file) const
			{
				Eigen::save(file);
			}

			//type
			Type Modal::type(void) const
			{
				return Type::Modal;
			}

			//sets
			uint32_t Modal::state_set(void) const
			{
				return (uint32_t) solvers::state::x;
			}
			uint32_t Modal::force_set(void) const
			{
				return (uint32_t) solvers::force::R;
			}
			uint32_t Modal::tangent_set(void) const
			{
				return
					(uint32_t) solvers::tangent::Z |
					(uint32_t) solvers::tangent::K |
					(uint32_t) solvers::tangent::M;
			}

			//solve
			void Modal::setup(void)
			{
				m_step_max = modes_count();
				Eigen::setup();
			}
			bool Modal::solve(void)
			{
				//apply
				m_assembler->apply_supports();
				m_assembler->apply_initials();
				//compute
				m_assembler->compute();
				//assemble
				m_assembler->assemble();
				//solve
				if(!eigen_gen())
				{
					printf("\tModal decomposition failed!\n");
					return false;
				}
				//record
				while(m_step < modes_count())
				{
					//interface
					if(m_stop) return false;
					if(m_interface) m_interface();
					//apply
					compute_state();
					m_assembler->apply_state();
					m_assembler->compute();
					m_assembler->record();
					//print
					const double w = sqrt(m_e[m_step]);
					const double f = sqrt(m_e[m_step]) / 2 / M_PI;
					printf("Mode: %04d | Angular frequency: %+.2e | Frequency: %+.2e | Dof: %+.2e |\n", m_step, w, f, m_watch_dof.value());
					m_step++;
				}
				//return
				return true;
			}
			void Modal::record(void)
			{
				m_state_data[m_step] = m_e[m_step];
			}

			void Modal::compute_state(void)
			{
				//data
				const uint16_t nu = m_analysis->assembler()->dof_unknow();
				//state
				for(uint32_t i = 0; i < nu; i++)
				{
					m_x_new[i] = m_scale * m_z[i + nu * m_step];
				}
			}
		}
	}
}
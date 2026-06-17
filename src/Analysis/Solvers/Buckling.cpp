//std
#include <cstring>
#include <climits>

//Math
#include "Math/inc/Linear/Sparse.hpp"
#include "Math/inc/Linear/Matrix.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/Buckling.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Buckling::Buckling(void)
			{
				return;
			}

			//destructor
			Buckling::~Buckling(void)
			{
				return;
			}

			//serialization
			void Buckling::load(FILE* file)
			{
				Eigen::load(file);
			}
			void Buckling::save(FILE* file) const
			{
				Eigen::save(file);
			}

			//type
			Type Buckling::type(void) const
			{
				return Type::Buckling;
			}

			//sets
			uint32_t Buckling::state_set(void) const
			{
				return (uint32_t) solvers::state::x;
			}
			uint32_t Buckling::force_set(void) const
			{
				return 
					(uint32_t) solvers::force::R |
					(uint32_t) solvers::force::fd |
					(uint32_t) solvers::force::fi |
					(uint32_t) solvers::force::fr;
			}
			uint32_t Buckling::tangent_set(void) const
			{
				return 
					(uint32_t) solvers::tangent::Z | 
					(uint32_t) solvers::tangent::K |
					(uint32_t) solvers::tangent::M;
			}

			//analysis
			bool Buckling::solve(void)
			{
				//data
				m_step = 0;
				const char* name = m_combination == UINT_MAX ? "Stiffness" : "Buckling load";
				//apply
				m_assembler->apply_supports();
				m_assembler->apply_initials();
				//compute
				m_assembler->compute_joints();
				m_assembler->compute_elements();
				m_assembler->compute_constraints();
				//stiffness
				m_assembler->assemble_stiffness();
				m_assembler->assemble_dead_force();
				m_assembler->assemble_internal_force();
				m_assembler->assemble_reference_force();
				//mode
				if(m_combination == UINT_MAX)
				{
					if(!eigen_std())
					{
						printf("\tBuckling decomposition failed!\n");
						return false;
					}
				}
				else
				{
					if(!compute_buckling())
					{
						return false;
					}
				}
				//record
				while(m_step < modes_count() && m_step < m_mode_crop)
				{
					//interface
					if(m_stop) return false;
					if(m_interface) m_interface();
					//state
					compute_state();
					m_assembler->apply_state();
					//compute
					m_assembler->compute_joints();
					m_assembler->compute_elements();
					m_assembler->compute_constraints();
					//assemble
					m_assembler->assemble_internal_force();
					//record
					compute_reactions();
					m_assembler->record();
					printf("Mode: %04d | %s: %+.2e | Dof: %+.2e |\n", m_step, name, m_e[m_step], m_watch_dof.value());
					m_step++;
				}
				//return
				return true;
			}

			void Buckling::record(void)
			{
				m_state_data[m_step] = m_e[m_step];
			}

			void Buckling::compute_state(void)
			{
				const uint32_t nu = m_analysis->assembler()->dof_unknow();
				for(uint32_t i = 0; i < nu; i++)
				{
					m_x_new[i] = m_scale * m_z[i + nu * m_step];
				}
			}
			bool Buckling::compute_buckling(void)
			{
				//data
				const uint32_t nu = m_assembler->dof_unknow();
				const int32_t* c = m_assembler->cols_map();
				memcpy(m_M, m_K, c[nu] * sizeof(double));
				//solve
				for(uint32_t i = 0; i < nu; i++)
				{
					m_fru[i] += m_fdu[i] - m_fiu[i];
				}
				if(!linear_solve(m_dx, m_K, m_fru))
				{
					printf("Unable to decompose stiffness matrix!\n");
					return false;
				}
				//stiffness
				m_assembler->increment_state();
				//compute
				m_analysis->assembler()->compute_joints();
				m_analysis->assembler()->compute_elements();
				m_analysis->assembler()->compute_constraints();
				//assemble
				m_assembler->assemble_stiffness();
				//solve
				for(int32_t i = 0; i < c[nu]; i++)
				{
					m_K[i] -= m_M[i];
				}
				if(!eigen_gen())
				{
					printf("\tBuckling decomposition failed!\n");
					return false;
				}
				for(uint32_t i = 0; i < modes_count(); i++)
				{
					if(m_e[i] < 0)
					{
						m_e[i] = -1 / m_e[i];
					}
					else
					{
						m_mode_crop = i;
						break;
					}
				}
				//return
				return true;
			}
			void Buckling::compute_reactions(void)
			{
				const uint32_t nk = m_analysis->assembler()->dof_know();
				for(uint32_t i = 0; i < nk; i++)
				{
					m_R[i] = m_fik[i] - m_frk[i] - m_fdk[i];
				}
			}
		}
	}
}
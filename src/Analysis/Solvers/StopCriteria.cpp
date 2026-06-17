//std
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"

//FEA
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"
#include "FEA/inc/Analysis/Solvers/StopCriteria.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructor
			StopCriteria::StopCriteria(Solver* solver) : m_types(0), m_solver{solver}
			{
				return;
			}

			//destructor
			StopCriteria::~StopCriteria(void)
			{
				return;
			}

			//serialization
			void StopCriteria::load(FILE* file)
			{
				if(fscanf(file, "%d", &m_types) != 1)
				{
					throw std::runtime_error("Stop criteria loading error!");
				}
			}
			void StopCriteria::save(FILE* file) const
			{
				if(fprintf(file, "%d ", m_types) < 0)
				{
					throw std::runtime_error("Stop criteria saving error!");
				}
			}

			//data
			uint32_t StopCriteria::types(Type type)
			{
				return m_types |= uint32_t(type);
			}
			uint32_t StopCriteria::types(void) const
			{
				return m_types;
			}
			uint32_t StopCriteria::types(uint32_t types)
			{
				return m_types = types;
			}

			//compute
			bool StopCriteria::compute(void)
			{
				return 
					compute_step() || 
					compute_attempt() || 
					compute_watch_dof() || 
					compute_load_limits() || compute_time_limits() || 
					compute_load_minimum() || compute_load_maximum() || 
					compute_load_negative() || compute_load_positive() ||
					compute_state_minimum() || compute_state_maximum() ||
					compute_state_negative() || compute_state_positive();
			}

			//names
			const char* StopCriteria::type_name(Type type)
			{
				const char* names[] = {
					"Load minimum", "Load maximum", "Load negative", "Load positive",
					"State minimum", "State maximum", "State negative", "State positive"
				};
				return names[math::bit_index(uint32_t(type))];
			}

			//compute
			bool StopCriteria::compute_step(void) const
			{
				if(m_solver->m_step >= m_solver->m_step_max)
				{
					printf("\tMax steps reached!\n");
					return true;
				}
				return false;
			}
			bool StopCriteria::compute_attempt(void) const
			{
				//data
				Nonlinear* solver = dynamic_cast<Nonlinear*>(m_solver);
				//compute
				if(solver && solver->m_attempt >= solver->m_attempt_max)
				{
					printf("\tSolution not found in max attempts!\n");
					return true;
				}
				return false;
			}
			bool StopCriteria::compute_watch_dof(void) const
			{
				if(m_solver->m_watch_dof.value() < m_solver->m_watch_dof.value_min())
				{
					printf("\tWatch dof minimum value reached!\n");
					return true;
				}
				if(m_solver->m_watch_dof.value() > m_solver->m_watch_dof.value_max())
				{
					printf("\tWatch dof maximum value reached!\n");
					return true;
				}
				return false;
			}
			bool StopCriteria::compute_load_limits(void) const
			{
				if(m_solver->m_p_new < m_solver->m_p_min)
				{
					printf("\tLoad minimum value reached!\n");
					return true;
				}
				if(m_solver->m_p_new > m_solver->m_p_max)
				{
					printf("\tLoad maximum value reached!\n");
					return true;
				}
				return false;
			}
			bool StopCriteria::compute_time_limits(void) const
			{
				if(m_solver->m_t_new < m_solver->m_t_min)
				{
					printf("\tTime minimum value reached!\n");
					return true;
				}
				if(m_solver->m_t_new > m_solver->m_t_max)
				{
					printf("\tTime maximum value reached!\n");
					return true;
				}
				return false;
			}
			bool StopCriteria::compute_load_minimum(void) const
			{
				const double p = m_solver->m_p_new;
				const uint32_t s = m_solver->m_step;
				const double* d = m_solver->m_state_data;
				if(m_types & uint32_t(Type::LoadMinimum) && s > 1 && d[s - 1] < p && d[s - 1] < d[s - 2])
				{
					printf("\tLoad minimum limit point reached!\n");
					return true;
				}
				return false;
			}
			bool StopCriteria::compute_load_maximum(void) const
			{
				const double p = m_solver->m_p_new;
				const uint32_t s = m_solver->m_step;
				const double* d = m_solver->m_state_data;
				if(m_types & uint32_t(Type::LoadMaximum) && s > 1 && d[s - 1] > p && d[s - 1] > d[s - 2])
				{
					printf("\tLoad maximum limit point reached!\n");
					return true;
				}
				return false;
			}
			bool StopCriteria::compute_load_negative(void) const
			{
				const double p = m_solver->m_p_new;
				const uint32_t s = m_solver->m_step;
				const double* d = m_solver->m_state_data;
				if(m_types & uint32_t(Type::LoadNegative) && p < 0 && d[s - 1] > 0)
				{
					printf("\tLoad negative value reached!\n");
					return true;
				}
				return false;
			}
			bool StopCriteria::compute_load_positive(void) const
			{
				const double p = m_solver->m_p_new;
				const uint32_t s = m_solver->m_step;
				const double* d = m_solver->m_state_data;
				if(m_types & uint32_t(Type::LoadPositive) && p > 0 && d[s - 1] < 0)
				{
					printf("\tLoad positive value reached!\n");
					return true;
				}
				return false;
			}
			bool StopCriteria::compute_state_minimum(void) const
			{
				return false;
			}
			bool StopCriteria::compute_state_maximum(void) const
			{
				return false;
			}
			bool StopCriteria::compute_state_negative(void) const
			{
				return false;
			}
			bool StopCriteria::compute_state_positive(void) const
			{
				return false;
			}
		}
	}
}
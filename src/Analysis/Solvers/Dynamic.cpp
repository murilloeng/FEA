//std
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vector.hpp"
#include "Math/inc/Linear/Sparse.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Dynamic.hpp"
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Dynamic::Dynamic(void) : m_integration(solvers::Integration::newmark)
			{
				return;
			}

			//destructor
			Dynamic::~Dynamic(void)
			{
				return;
			}

			//serialization
			void Dynamic::load(FILE* file)
			{
				Time::load(file);
				m_newmark.load(file);
				uint32_t integration;
				if(fscanf(file, "%d", &integration) != 1)
				{
					throw std::runtime_error("Unable to read Dynamic solver data from file!");
				}
				m_integration = solvers::Integration(integration);
			}
			void Dynamic::save(FILE* file) const
			{
				Time::save(file);
				m_newmark.save(file);
				if(fprintf(file, "%d ", uint32_t(m_integration)) < 0)
				{
					throw std::runtime_error("Unable to write Dynamic solver data to file!");
				}
			}

			//sets
			uint32_t Dynamic::state_set(void) const
			{
				return
					(uint32_t) solvers::state::x |
					(uint32_t) solvers::state::v |
					(uint32_t) solvers::state::a |
					(uint32_t) solvers::state::t;
			}
			uint32_t Dynamic::force_set(void) const
			{
				return
					(uint32_t) solvers::force::r |
					(uint32_t) solvers::force::R |
					(uint32_t) solvers::force::fn |
					(uint32_t) solvers::force::fi |
					(uint32_t) solvers::force::fe;
			}
			uint32_t Dynamic::tangent_set(void) const
			{
				return m_integration == Integration::runge_kutta ? 
					uint32_t(solvers::tangent::M) :
					uint32_t(solvers::tangent::K) |
					uint32_t(solvers::tangent::C) |
					uint32_t(solvers::tangent::M) |
					uint32_t(solvers::tangent::S);
			}

			//data
			solvers::Newmark& Dynamic::newmark(void)
			{
				return m_newmark;
			}

			solvers::Integration Dynamic::integration(void) const
			{
				return m_integration;
			}
			solvers::Integration Dynamic::integration(solvers::Integration integration)
			{
				return m_integration = integration;
			}

			//analysis
			bool Dynamic::solve(void)
			{
				//data
				bool (Dynamic::*factory[])(void) = {
					&Dynamic::solve_newmark, 
					&Dynamic::solve_runge_kutta
				};
				const solvers::Integration items[] = {
					solvers::Integration::newmark,
					solvers::Integration::runge_kutta
				};
				//solve
				if(!compute_initial()) return false;
				for(uint32_t i = 0; i < sizeof(items) / sizeof(solvers::Integration); i++)
				{
					if(m_integration == items[i]) return (this->*factory[i])();
				}
				//return
				return false;
			}

			bool Dynamic::compute_initial(void)
			{
				//acceleration
				m_assembler->apply();
				m_assembler->compute();
				m_assembler->assemble();
				if(!compute_acceleration())
				{
					return false;
				}
				m_assembler->apply_acceleration();
				m_assembler->assemble_inertial_force();
				memcpy(m_a_old, m_a_new, m_assembler->dof_unknow() * sizeof(double));
				//assembler
				compute_reactions();
				m_assembler->record();
				m_assembler->update();
				//return
				return true;
			}
			bool Dynamic::compute_acceleration(void)
			{
				//data
				const uint32_t nu = m_assembler->dof_unknow();
				//residue
				for(uint32_t i = 0; i < nu; i++)
				{
					m_r[i] = m_feu[i] - m_fiu[i];
				}
				//return
				return linear_solve(m_a_new, m_M, m_r);
			}
		}
	}
}
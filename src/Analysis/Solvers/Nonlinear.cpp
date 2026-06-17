//std
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Time.hpp"
#include "FEA/inc/Analysis/Solvers/Nonlinear.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Nonlinear::Nonlinear(void) :
				m_attempt_max(5), m_iteration_max(10), m_iteration_desired(5), 
				m_convergence(this), m_continuation(this)
			{
				return;
			}

			//destructor
			Nonlinear::~Nonlinear(void)
			{
				return;
			}

			//serialization
			void Nonlinear::load(FILE* file)
			{
				//load
				m_convergence.load(file);
				m_continuation.load(file);
				if(fscanf(file, "%d %d %d", &m_attempt_max, &m_iteration_max, &m_iteration_desired) != 3)
				{
					throw std::runtime_error("Nonlinear solver loading error!");
				}
			}
			void Nonlinear::save(FILE* file) const
			{
				//save
				m_convergence.save(file);
				m_continuation.save(file);
				if(fprintf(file, "%d %d %d ", m_attempt_max, m_iteration_max, m_iteration_desired) < 0)
				{
					throw std::runtime_error("Nonlinear solver saving error!");
				}
			}

			//data
			uint32_t Nonlinear::attempt_max(void) const
			{
				return m_attempt_max;
			}
			uint32_t Nonlinear::attempt_max(uint32_t attempt_max)
			{
				return m_attempt_max = attempt_max;
			}
			uint32_t Nonlinear::iteration_max(void) const
			{
				return m_iteration_max;
			}
			uint32_t Nonlinear::iteration_max(uint32_t iteration_max)
			{
				return m_iteration_max = iteration_max;
			}
			uint32_t Nonlinear::iteration_desired(void) const
			{
				return m_iteration_desired;
			}
			uint32_t Nonlinear::iteration_desired(uint32_t iterations_desired)
			{
				return m_iteration_desired = iterations_desired;
			}

			Convergence& Nonlinear::convergence(void)
			{
				return m_convergence;
			}
			const Convergence& Nonlinear::convergence(void) const
			{
				return m_convergence;
			}
			Convergence& Nonlinear::convergence(Convergence::Type type)
			{
				m_convergence.m_type = type;
				return m_convergence;
			}

			Continuation& Nonlinear::continuation(void)
			{
				return m_continuation;
			}
			const Continuation& Nonlinear::continuation(void) const
			{
				return m_continuation;
			}
			Continuation& Nonlinear::continuation(Continuation::Type type)
			{
				m_continuation.m_type = type;
				return m_continuation;
			}

			//analysis
			void Nonlinear::setup(void)
			{
				m_attempt = 0;
				m_iteration = 0;
			}
		}
	}
}
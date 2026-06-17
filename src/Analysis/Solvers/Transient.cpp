//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Transient.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Transient::Transient(void)
			{
				return;
			}

			//destructor
			Transient::~Transient(void)
			{
				return;
			}

			//serialization
			void Transient::load(FILE* file)
			{
				Time::load(file);
				m_euller.load(file);
			}
			void Transient::save(FILE* file) const
			{
				Time::save(file);
				m_euller.save(file);
			}

			//data
			solvers::Euller& Transient::euller(void)
			{
				return m_euller;
			}
		}
	}
}
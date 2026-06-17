//std
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

//FEA
#include "FEA/inc/Analysis/Solvers/Euller.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Euller::Euller(void) : m_alpha(0)
			{
				return;
			}

			//destructor
			Euller::~Euller(void)
			{
				return;
			}

			//serialization
			void Euller::load(FILE* file)
			{
				if(fscanf(file, "%lf", &m_alpha) != 1)
				{
					throw std::runtime_error("Euller solver data loading error!");
				}
			}
			void Euller::save(FILE* file) const
			{
				fprintf(file, "%+.6e ", m_alpha);
			}

			//data
			double Euller::alpha(double alpha)
			{
				return m_alpha = alpha;
			}

			double Euller::alpha(void) const
			{
				return m_alpha;
			}
		}
	}
}
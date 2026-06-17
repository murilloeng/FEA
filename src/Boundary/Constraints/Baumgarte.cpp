//std
#include <cstdlib>
#include <cstdint>
#include <stdexcept>

//FEA
#include "FEA/inc/Boundary/Constraints/Baumgarte.hpp"

namespace fea
{
	namespace boundary
	{
		//constructors
		Baumgarte::Baumgarte(void) : m_beta{1}, m_alpha{1}
		{
			return;
		}

		//destructor
		Baumgarte::~Baumgarte(void)
		{
			return;
		}

		//serialization
		void Baumgarte::load(FILE* file)
		{
			if(fscanf(file, "%lf %lf", &m_beta, &m_alpha) != 2)
			{
				throw std::runtime_error("Baumgarte data loading error!");
			}
		}
		void Baumgarte::save(FILE* file) const
		{
			if(fprintf(file, "%+.6e %+.6e ", m_beta, m_alpha) < 0)
			{
				throw std::runtime_error("Baumgarte data saving error!");
			}
		}

		//data
		double Baumgarte::beta(void) const
		{
			return m_beta;
		}
		double Baumgarte::beta(double beta)
		{
			return m_beta = beta;
		}

		double Baumgarte::alpha(void) const
		{
			return m_alpha;
		}
		double Baumgarte::alpha(double alpha)
		{
			return m_alpha = alpha;
		}
	}
}
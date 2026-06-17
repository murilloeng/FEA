//std
#include <cstdlib>
#include <cstdint>
#include <stdexcept>

//FEA
#include "FEA/inc/Analysis/Solvers/Newmark.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Newmark::Newmark(void) : m_hht(false), m_beta(0.25), m_gamma(0.50), m_alpha(0.00)
			{
				return;
			}

			//destructor
			Newmark::~Newmark(void)
			{
				return;
			}

			//serialization
			void Newmark::load(FILE* file)
			{
				uint32_t hht;
				if(fscanf(file, "%d %lf %lf %lf", &hht, &m_beta, &m_gamma, &m_alpha) != 4)
				{
					throw std::runtime_error("Newmark solver data loading error!");
				}
				m_hht = hht;
			}
			void Newmark::save(FILE* file) const
			{
				fprintf(file, "%01d %+.6e %+.6e %+.6e ", m_hht, m_beta, m_gamma, m_alpha);
			}

			//data
			bool Newmark::hht(bool hht)
			{
				return m_hht = hht;
			}
			double Newmark::beta(double beta)
			{
				return m_beta = beta;
			}
			double Newmark::gamma(double gamma)
			{
				return m_gamma = gamma;
			}
			double Newmark::alpha(double alpha)
			{
				return m_alpha = alpha;
			}

			bool Newmark::hht(void) const
			{
				return m_hht;
			}
			double Newmark::beta(void) const
			{
				return m_beta;
			}
			double Newmark::gamma(void) const
			{
				return m_gamma;
			}
			double Newmark::alpha(void) const
			{
				return m_alpha;
			}

			//analysis
			void Newmark::parameters(double& a, double& b, double& g) const
			{
				a = m_hht ? m_alpha : 0;
				g = m_hht ? (1 + 2 * a) / 2 : m_gamma;
				b = m_hht ? (1 + a) * (1 + a) / 4 : m_beta;
			}
		}
	}
}
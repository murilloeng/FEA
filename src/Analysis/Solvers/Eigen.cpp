//std
#include <cmath>
#include <climits>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Linear/Sparse.hpp"
#include "Math/inc/Linear/Matrix.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Types.hpp"
#include "FEA/inc/Analysis/Solvers/Eigen.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

//lapack
extern "C"
{
	void dsyevx_(const char*, const char*, const char*, const uint32_t*, double*, const uint32_t*, const double*, const double*,
		const uint32_t*, const uint32_t*, const double*, const uint32_t*, double*, double*, const uint32_t*, double*, int32_t*, uint32_t*, uint32_t*, int32_t*);
	void dsygvx_(const uint32_t*, const char*, const char*, const char*, const uint32_t*, double*, const uint32_t*, double*, const uint32_t*,
		const double*, const double*, const uint32_t*, const uint32_t*, const double*, const uint32_t*, double*, double*, const uint32_t*, double*,
		const uint32_t*, uint32_t*, uint32_t*, int32_t*);
}

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			Eigen::Eigen(void) : m_scale(1), m_mode_crop(UINT_MAX), m_spectre_min(0), m_spectre_max(0), m_spectre(spectre::min)
			{
				return;
			}

			//destructor
			Eigen::~Eigen(void)
			{
				return;
			}

			//serialization
			void Eigen::load(FILE* file)
			{
				uint32_t spectre;
				Solver::load(file);
				if(fscanf(file, "%d %d %d %lf", &spectre, &m_spectre_min, &m_spectre_max, &m_scale) != 4)
				{
					throw std::runtime_error("Eigen solver loading error!");
				}
				m_spectre = solvers::spectre(spectre);
			}
			void Eigen::save(FILE* file) const
			{
				Solver::save(file);
				if(fprintf(file, "%d %d %d %+.6e ", (uint32_t) m_spectre, m_spectre_min, m_spectre_max, m_scale) < 0)
				{
					throw std::runtime_error("Eigen solver saving error!");
				}
			}

			//data
			double Eigen::scale(void) const
			{
				return m_scale;
			}
			double Eigen::scale(double scale)
			{
				return m_scale = scale;
			}

			uint32_t Eigen::spectre_min(void) const
			{
				return m_spectre_min;
			}
			uint32_t Eigen::spectre_max(void) const
			{
				return m_spectre_max;
			}
			uint32_t Eigen::spectre_min(uint32_t spectre_min)
			{
				return m_spectre_min = spectre_min;
			}
			uint32_t Eigen::spectre_max(uint32_t spectre_max)
			{
				return m_spectre_max = spectre_max;
			}

			solvers::spectre Eigen::spectre(void) const
			{
				return m_spectre;
			}
			solvers::spectre Eigen::spectre(solvers::spectre spectre)
			{
				return m_spectre = spectre;
			}

			//eigen
			bool Eigen::eigen_std(void) const
			{
				//size
				uint32_t n = m_assembler->dof_unknow();
				const int32_t* c = m_assembler->cols_map();
				const int32_t* r = m_assembler->rows_map();
				//data
				int32_t status;
				double a = 0;
				int32_t m = 8 * n;
				const uint32_t p = modes_count();
				double* w = (double*) alloca(m * sizeof(double));
				uint32_t* u = (uint32_t*) alloca(n * sizeof(uint32_t));
				uint32_t* q = (uint32_t*) alloca(5 * n * sizeof(double));
				//solve
				const char s = m_spectre == spectre::full ? 'A' : 'I';
				math::Matrix(m_k, n, n) = math::Sparse(m_K, r, c, n, n).convert();
				uint32_t il = m_spectre == spectre::partial ? m_spectre_min + 1 : m_spectre == spectre::min ? 1 : n;
				uint32_t iu = m_spectre == spectre::partial ? m_spectre_max + 1 : m_spectre == spectre::min ? 1 : n;
				dsyevx_("V", &s, "L", &n, m_k, &n, nullptr, nullptr, &il, &iu, &a, &p, m_e, m_z, &n, w, &m, q, u, &status);
				//return
				return status == 0;
			}
			bool Eigen::eigen_gen(void) const
			{
				//size
				uint32_t n = m_assembler->dof_unknow();
				const int32_t* c = m_assembler->cols_map();
				const int32_t* r = m_assembler->rows_map();
				//data
				int32_t status;
				double a = 0;
				uint32_t m = 8 * n;
				const uint32_t t = 1;
				const uint32_t p = modes_count();
				double* w = (double*) alloca(m * sizeof(double));
				uint32_t* u = (uint32_t*) alloca(n * sizeof(uint32_t));
				uint32_t* q = (uint32_t*) alloca(5 * n * sizeof(double));
				//solve
				const char s = m_spectre == spectre::full ? 'A' : 'I';
				math::Matrix(m_k, n, n) = math::Sparse(m_K, r, c, n, n).convert();
				math::Matrix(m_m, n, n) = math::Sparse(m_M, r, c, n, n).convert();
				uint32_t il = m_spectre == spectre::partial ? m_spectre_min + 1 : m_spectre == spectre::min ? 1 : n;
				uint32_t iu = m_spectre == spectre::partial ? m_spectre_max + 1 : m_spectre == spectre::min ? 1 : n;
				dsygvx_(&t, "V", &s, "L", &n, m_k, &n, m_m, &n, nullptr, nullptr, &il, &iu, &a, &p, m_e, m_z, &n, w, &m, q, u, &status);
				//return
				return status == 0;
			}
			uint32_t Eigen::modes_count(void) const
			{
				return 
					m_spectre == spectre::min || m_spectre == spectre::max ? 1 :
					m_spectre == spectre::full ? m_assembler->dof_unknow() : 
					m_spectre == spectre::partial ? m_spectre_max - m_spectre_min + 1 : 0;
			}
		}
	}
}
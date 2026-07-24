#pragma once

//std
#include <cstdint>

//Math
#include "Math/inc/Solvers/Solver.hpp"
#include "Math/inc/Eigen/SparseSymStd.hpp"
#include "Math/inc/Eigen/SparseSymGen.hpp"

//FEA
#include "FEA/inc/Analysis/Solvers/WatchDOF.hpp"

namespace fea
{
	namespace analysis
	{
		class Analysis;
		class Assembler;
	}
}

namespace fea
{
	namespace analysis
	{
		class Solver : virtual public math::solvers::Solver
		{
		protected:
			//constructor
			Solver(void);

			//destructor
			~Solver(void);

		public:
			//data
			WatchDOF& watch_dof(void);

			uint32_t load_combination(uint32_t);
			uint32_t load_combination(void) const;

			math::eigen::SparseSymStd& eigen_std(void);
			math::eigen::SparseSymGen& eigen_gen(void);

		protected:
			//analysis
			void check(void) override;
			void setup(void) override;
			void allocate(void) override;

			//data
			double *m_s, *m_U;
			WatchDOF m_watch_dof;
			uint32_t m_load_combination;
			static Analysis* m_analysis;
			math::eigen::SparseSymStd m_eigen_std;
			math::eigen::SparseSymGen m_eigen_gen;

			using math::solvers::Solver::size;
			using math::solvers::Solver::rows_map;
			using math::solvers::Solver::cols_map;
			using math::solvers::Solver::watch_dof;

			//friends
			friend class fea::analysis::Analysis;
			friend class fea::analysis::Assembler;
		};
	}
}
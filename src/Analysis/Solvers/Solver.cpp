//std
#include <cstring>
#include <filesystem>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Solver::Solver(void) : m_s{nullptr}, m_U{nullptr}, m_load_combination{UINT32_MAX}
		{
			return;
		}

		//destructor
		Solver::~Solver(void)
		{
			delete[] m_s;
			delete[] m_U;
		}

		//data
		WatchDOF& Solver::watch_dof(void)
		{
			return m_watch_dof;
		}

		uint32_t Solver::load_combination(void) const
		{
			return m_load_combination;
		}
		uint32_t Solver::load_combination(uint32_t load_combination)
		{
			return m_load_combination = load_combination;
		}

		math::eigen::SparseSymStd& Solver::eigen_std(void)
		{
			return m_eigen_std;
		}
		math::eigen::SparseSymGen& Solver::eigen_gen(void)
		{
			return m_eigen_gen;
		}

		//analysis
		void Solver::check(void)
		{
			m_watch_dof.check();
		}
		void Solver::setup(void)
		{
			cleanup();
			allocate();
			math::solvers::Solver::setup();
			math::solvers::Solver::m_watch_dof = m_analysis->model()->mesh()->node(m_watch_dof.m_node)->dof_index(m_watch_dof.m_dof);
		}
		void Solver::allocate(void)
		{
			//data
			const uint32_t nm = m_eigen_std.modes();
			const uint32_t nu = m_analysis->m_assembler->dof_unknow();
			//setup
			delete[] m_s;
			delete[] m_U;
			m_s = new double[nu];
			m_U = new double[nu * nm];
			math::solvers::Solver::allocate(nu);
			//setup
			memset(m_x_old, 0, m_size * sizeof(double));
		}

		void Solver::model_update(void) const
		{
			m_analysis->m_model->update();
		}
		void Solver::model_restore(void) const
		{
			m_analysis->m_model->restore();
		}
		void Solver::model_compute(void) const
		{
			m_analysis->m_model->compute();
		}

		//static
		Analysis* Solver::m_analysis = nullptr;
	}
}
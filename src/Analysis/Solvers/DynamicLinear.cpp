//std
#include <cstdio>
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Sparse.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/DynamicLinear.hpp"

//x: state
//v: velocity
//a: acceleration

//target system: M * a + C * v + K * x = fe(t) - fs(t)

//fs(t): internal and constraint forces due to prescribed supports

//time interpolation:
//v_new = v_old + (1 - g) * dt * a_old + g * dt * a_new
//x_new = x_old + dt * v_old + (1 / 2 - b) * dt * dt * a_old + b * dt * dt * a_new

//effective system: S * a_new = fe(t_new) - fs(t_new) - C * v_p - K * x_p
//S = M + g * dt * C + b * dt * dt * K

namespace fea
{
	namespace analysis
	{
		//constructor
		DynamicLinear::DynamicLinear(void) : m_g{0.50}, m_b{0.25}, m_S{nullptr}
		{
			return;
		}

		//destructor
		DynamicLinear::~DynamicLinear(void)
		{
			delete m_S;
		}

		//solve
		void DynamicLinear::solve(void)
		{
			//decompose
			if(!m_S->decompose())
			{
				throw std::runtime_error("Error: Dynamic Linear solver effective matrix decomposition failed!");
			}
			//solve
			print();
			record();
			for(m_step = 1; !stop(); m_step++)
			{
				compute_step();
				if(!m_status)
				{
					m_status = true;
					if(!m_silent) printf("Solver failed in step %d!\n", m_step);
					break;
				}
				print();
				if(m_callback_step) m_callback_step();
			}
			//release
			m_S->release();
		}
		void DynamicLinear::compute_step(void)
		{
			//time
			m_t_new = m_t_old + m_dt;
			//predictor
			compute_loads();
			for(uint32_t i = 0; i < m_size; i++)
			{
				m_v_new[i] = m_v_old[i] + (1 - m_g) * m_dt * m_a_old[i];
				m_x_new[i] = m_x_old[i] + m_dt * m_v_old[i] + (0.5 - m_b) * m_dt * m_dt * m_a_old[i];
			}
			//residue
			for(uint32_t i = 0; i < m_size; i++)
			{
				m_r[i] = m_fe[i] - m_fi[i];
			}
			for(uint32_t j = 0; j < m_size; j++)
			{
				for(int32_t k = m_cols_map[j]; k < m_cols_map[j + 1]; k++)
				{
					m_r[m_rows_map[k]] -= m_K[k] * m_x_new[j] + m_C[k] * m_v_new[j];
				}
			}
			//solve
			m_status = m_S->substitute(m_a_new, m_r, 1);
			if(!m_status)
			{
				if(!m_silent) printf("Unable to substitute effective system in step!\n");
				return;
			}
			//corrector
			for(uint32_t i = 0; i < m_size; i++)
			{
				m_v_new[i] += m_g * m_dt * m_a_new[i];
				m_x_new[i] += m_b * m_dt * m_dt * m_a_new[i];
			}
			compute_energy();
			//update
			update();
			record();
		}

		//data
		uint32_t DynamicLinear::states_set(void) const
		{
			return
				1 << uint32_t(math::solvers::Solver::State::x)|
				1 << uint32_t(math::solvers::Solver::State::v)|
				1 << uint32_t(math::solvers::Solver::State::a)|
				1 << uint32_t(math::solvers::Solver::State::t);
		}
		uint32_t DynamicLinear::forces_set(void) const
		{
			return
				1 << uint32_t(math::solvers::Solver::Force::r)|
				1 << uint32_t(math::solvers::Solver::Force::fi)|
				1 << uint32_t(math::solvers::Solver::Force::fe);
		}
		uint32_t DynamicLinear::energies_set(void) const
		{
			return
				1 << uint32_t(math::solvers::Solver::Energy::T)|
				1 << uint32_t(math::solvers::Solver::Energy::U)|
				1 << uint32_t(math::solvers::Solver::Energy::V);
		}
		uint32_t DynamicLinear::tangents_set(void) const
		{
			return
				1 << uint32_t(math::solvers::Solver::Tangent::K)|
				1 << uint32_t(math::solvers::Solver::Tangent::C)|
				1 << uint32_t(math::solvers::Solver::Tangent::M);
		}

		double DynamicLinear::beta(void) const
		{
			return m_b;
		}
		double DynamicLinear::beta(double beta)
		{
			return m_b = beta;
		}

		double DynamicLinear::gamma(void) const
		{
			return m_g;
		}
		double DynamicLinear::gamma(double gamma)
		{
			return m_g = gamma;
		}

		//analysis
		void DynamicLinear::check(void)
		{
			Solver::check();
		}
		void DynamicLinear::setup(void)
		{
			//setup
			Solver::setup();
			math::solvers::Incremental::setup();
			//system
			delete m_S;
			m_S = new math::Sparse(m_size, m_size);
			m_analysis->assembler()->request().m_f = true;
			m_analysis->assembler()->request().m_K = true;
			m_analysis->assembler()->request().m_C = true;
			m_analysis->assembler()->request().m_M = true;
			m_analysis->assembler()->request().m_time = true;
			//setup
			reset();
			m_analysis->assembler()->assemble();
			m_analysis->assembler()->request().m_K = false;
			m_analysis->assembler()->request().m_C = false;
			m_analysis->assembler()->request().m_M = false;
			//acceleration
			for(uint32_t i = 0; i < m_size; i++)
			{
				m_r[i] = m_fe[i] - m_fi[i];
			}
			if(!math::solvers::Solver::solve(m_M, m_r, m_a_new))
			{
				memset(m_a_new, 0, m_size * sizeof(double));
				if(!m_silent) printf("Unable to compute acceleration in setup!\n");
			}
			memcpy(m_a_old, m_a_new, m_size * sizeof(double));
			//assemble
			m_S->pattern(m_rows_map, m_cols_map);
			for(int32_t i = 0; i < m_cols_map[m_size]; i++)
			{
				(*m_S)[i] = m_M[i] + m_g * m_dt * m_C[i] + m_b * m_dt * m_dt * m_K[i];
			}
			//energy
			compute_energy();
		}
		void DynamicLinear::print(void)
		{
			if(!m_silent) printf("Step: %4d Time: %+.6e State: %+.6e\n", m_step, m_t_new, m_watch_dof.state());
		}

		//compute
		void DynamicLinear::compute_loads(void)
		{
			//state
			memset(m_x_new, 0, m_size * sizeof(double));
			memset(m_v_new, 0, m_size * sizeof(double));
			memset(m_a_new, 0, m_size * sizeof(double));
			//forces
			memset(m_fi, 0, m_size * sizeof(double));
			memset(m_fe, 0, m_size * sizeof(double));
			m_analysis->assembler()->assemble_loads();
			m_analysis->assembler()->assemble_elements();
			m_analysis->assembler()->assemble_constraints();
		}
		void DynamicLinear::compute_energy(void)
		{
			m_T = m_U = m_V = 0;
			for(uint32_t j = 0; j < m_size; j++)
			{
				m_V -= m_fe[j] * m_x_new[j];
				for(int32_t k = m_cols_map[j]; k < m_cols_map[j + 1]; k++)
				{
					m_T += m_M[k] * m_v_new[m_rows_map[k]] * m_v_new[j] / 2;
					m_U += m_K[k] * m_x_new[m_rows_map[k]] * m_x_new[j] / 2;
				}
			}
		}
	}
}
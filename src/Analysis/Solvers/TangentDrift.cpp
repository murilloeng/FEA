//std
#include <ctime>
#include <cstring>

//Math
#include "Math/inc/Linear/Quat.hpp"
#include "Math/inc/Linear/Sparse.hpp"
#include "Math/inc/Miscellaneous/util.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/TangentDrift.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		TangentDrift::TangentDrift(void) : m_type{Type::InternalForce}, m_tests{10000}, m_dof_min{-1}, m_dof_max{+1}, m_dof_shift{1e-5}, m_tolerance{1e-05}
		{
			return;
		}

		//destructor
		TangentDrift::~TangentDrift(void)
		{
			return;
		}

		//solve
		void TangentDrift::solve(void)
		{
			srand(time(nullptr));
			m_type == Type::InternalForce ? solve_internal_force() : solve_stiffness();
		}

		//data
		uint32_t TangentDrift::tests(void) const
		{
			return m_tests;
		}
		uint32_t TangentDrift::tests(uint32_t tests)
		{
			return m_tests = tests;
		}

		double TangentDrift::dof_min(void) const
		{
			return m_dof_min;
		}
		double TangentDrift::dof_min(double dof_min)
		{
			return m_dof_min = dof_min;
		}

		double TangentDrift::dof_max(void) const
		{
			return m_dof_max;
		}
		double TangentDrift::dof_max(double dof_max)
		{
			return m_dof_max = dof_max;
		}

		double TangentDrift::dof_shift(void) const
		{
			return m_dof_shift;
		}
		double TangentDrift::dof_shift(double dof_shift)
		{
			return m_dof_shift = dof_shift;
		}

		double TangentDrift::tolerance(void) const
		{
			return m_tolerance;
		}
		double TangentDrift::tolerance(double tolerance)
		{
			return m_tolerance = tolerance;
		}

		TangentDrift::Type TangentDrift::type(Type type)
		{
			return m_type = type;
		}
		TangentDrift::Type TangentDrift::type(void) const
		{
			return m_type;
		}

		//data
		uint32_t TangentDrift::state_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::State::x);
		}
		uint32_t TangentDrift::force_set(void) const
		{
			return 0;
		}
		uint32_t TangentDrift::tangent_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::Tangent::K);
		}

		//compute
		void TangentDrift::compute_state(double* x)
		{
			//data
			const double d_min = m_dof_min;
			const double d_max = m_dof_max;
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			//state
			for(uint32_t i = 0; i < nu; i++)
			{
				x[i] = m_x_new[i] = d_min + (d_max - d_min) * rand() / RAND_MAX;
			}
			//quaternions
			for(mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
			{
				if(node->quaternion_new()) math::Quat(node->quaternion_new()).randu();
			}
		}

		//solve
		void TangentDrift::solve_stiffness(void)
		{
			//data
			const int32_t* c = m_cols_map;
			const int32_t* r = m_rows_map;
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			std::function<void(double*, const double*)> function = [this, nu] (double* f, const double* x) {
				memcpy(m_x_new, x, nu * sizeof(double));
				m_analysis->model()->compute();
				m_analysis->assembler()->assemble_internal_force(f);
			};
			//solve
			double* x = new double[nu];
			double* Ka = new double[c[nu]];
			double* Kr = new double[c[nu]];
			double* Kn = new double[nu * nu];
			for(uint32_t test = 0; test < m_tests; test++)
			{
				compute_state(x);
				m_analysis->model()->compute();
				m_analysis->assembler()->assemble_stiffness(Ka);
				math::ndiff(function, Kn, x, nu, nu, m_dof_shift);
				for(uint32_t i = 0; i < nu; i++)
				{
					for(int32_t j = c[i]; j < c[i + 1]; j++)
					{
						Kr[j] = Ka[j] - Kn[r[j] + nu * i];
					}
				}
				if(math::Sparse(Kr, r, c, nu, nu).norm() < m_tolerance * math::Sparse(Ka, r, c, nu, nu).norm())
				{
					printf("Stiffness Test: %d Status: OK! Error: %+.2e\n", test, math::Sparse(Kr, r, c, nu, nu).norm());
				}
				else
				{
					math::Matrix(Kn, nu, nu).print("Kn", m_tolerance);
					math::Sparse(Ka, r, c, nu, nu).print("Ka", true, m_tolerance);
					math::Sparse(Kr, r, c, nu, nu).print("Kr", true, m_tolerance);
					break;
				}
			}
			//delete
			delete[] x;
			delete[] Ka;
			delete[] Kn;
			delete[] Kr;
		}
		void TangentDrift::solve_internal_force(void)
		{
			//data
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			std::function<void(double*, const double*)> function = [this, nu] (double* U, const double* x) {
				memcpy(m_x_new, x, nu * sizeof(double));
				m_analysis->model()->compute();
				m_analysis->assembler()->assemble_internal_energy(*U);
			};
			//solve
			double* x = new double[nu];
			double* fa = new double[nu];
			double* fn = new double[nu];
			double* fr = new double[nu];
			for(uint32_t test = 0; test < m_tests; test++)
			{
				compute_state(x);
				m_analysis->model()->compute();
				m_analysis->assembler()->assemble_internal_force(fa);
				math::ndiff(function, fn, x, 1, nu, m_dof_shift);
				for(uint32_t i = 0; i < nu; i++) fr[i] = fa[i] - fn[i];
				if(math::Vector(fr, nu).norm() < m_tolerance * math::Vector(fa, nu).norm())
				{
					printf("Internal Force Test: %d Status: OK! Error: %+.2e\n", test, math::Vector(fr, nu).norm());
				}
				else
				{
					math::Vector(fn, nu).print("fn", m_tolerance);
					math::Vector(fa, nu).print("fa", m_tolerance);
					math::Vector(fr, nu).print("fr", m_tolerance);
					break;
				}
			}
			//delete
			delete[] x;
			delete[] fa;
			delete[] fn;
			delete[] fr;
		}
	}
}
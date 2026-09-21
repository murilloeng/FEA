//std
#include <ctime>

//Math
#include "Math/inc/Linear/Quat.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/TangentDrift.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		TangentDrift::TangentDrift(void)
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
			for(uint32_t i = 0; i < m_tests; i++)
			{
				
			}
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

		double TangentDrift::tolerance(void) const
		{
			return m_tolerance;
		}
		double TangentDrift::tolerance(double tolerance)
		{
			return m_tolerance = tolerance;
		}

		//data
		uint32_t TangentDrift::state_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::State::x);
		}
		uint32_t TangentDrift::force_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::Force::fi);
		}
		uint32_t TangentDrift::tangent_set(void) const
		{
			return 1 << uint32_t(math::solvers::Solver::Tangent::K);
		}

		//compute
		void TangentDrift::compute_state(void)
		{
			//data
			const double d_min = m_dof_min;
			const double d_max = m_dof_max;
			const uint32_t nu = m_analysis->assembler()->dof_unknow();
			//state
			for(uint32_t i = 0; i < nu; i++)
			{
				m_x_new[i] = d_min + (d_max - d_min) * rand() / RAND_MAX;
			}
			//quaternions
			for(mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
			{
				if(node->quaternion_new())
				{
					math::Quat(node->quaternion_new()).randu();
				}
			}
		}
	}
}
//std
#include <algorithm>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Initials/Initial.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace boundary
	{
		//costructors
		Initial::Initial(void)
		{
			return;
		}

		//destructor
		Initial::~Initial(void)
		{
			return;
		}

		//serialization
		void Initial::load(FILE* file)
		{
			uint32_t dof;
			if(fscanf(file, "%d %d %lf %lf", &m_node, &dof, &m_state, &m_velocity) != 4)
			{
				throw std::runtime_error("Initial loading error!");
			}
			m_dof = mesh::nodes::dof(dof);
		}
		void Initial::save(FILE* file) const
		{
			fprintf(file, "%04d %02d %+.6e %+.6e", m_node, (uint32_t) m_dof, m_state, m_velocity);
		}

		//data
		Boundary* Initial::boundary(void)
		{
			return m_boundary;
		}

		mesh::nodes::dof Initial::dof(void) const
		{
			return m_dof;
		}
		mesh::nodes::dof Initial::dof(mesh::nodes::dof dof)
		{
			return m_dof = dof;
		}
		mesh::nodes::Node* Initial::node(void) const
		{
			return m_boundary->model()->mesh()->node(m_node);
		}
		mesh::nodes::Node* Initial::node(uint32_t node)
		{
			return m_boundary->model()->mesh()->node(m_node = node);
		}

		double Initial::state(void) const
		{
			return m_state;
		}
		double Initial::state(double state)
		{
			return m_state = state;
		}
		double Initial::velocity(void) const
		{
			return m_velocity;
		}
		double Initial::velocity(double velocity)
		{
			return m_velocity = velocity;
		}

		//index
		uint32_t Initial::index(void) const
		{
			return m_index;
		}
		uint32_t Initial::index_node(void) const
		{
			return m_node;
		}

		//analysis
		bool Initial::check(void) const
		{
			//check node
			if(m_node >= m_boundary->model()->mesh()->nodes().size())
			{
				printf("Initial %04d has out of range node!\n", m_index);
				return false;
			}
			return true;
		}
		void Initial::setup(void)
		{
			//set dof
			const mesh::nodes::Node* node = m_boundary->model()->mesh()->node(m_node);
			const uint8_t p = math::bit_index(node->m_dof_set, (uint32_t) m_dof);
			m_dof_index = node->m_dof_index[p];
		}
		void Initial::apply_dof(void) const
		{
			m_boundary->model()->mesh()->node(m_node)->apply_dof(m_dof);
		}

		//analysis
		void Initial::apply(void) const
		{
			//data
			double* x = m_boundary->model()->analysis()->solver()->m_x_new;
			double* v = m_boundary->model()->analysis()->solver()->m_v_new;
			const uint32_t solver_set = m_boundary->model()->analysis()->solver()->state_set();
			//apply
			if(solver_set & (uint32_t) analysis::solvers::state::x) x[m_dof_index] = m_state;
			if(solver_set & (uint32_t) analysis::solvers::state::v) v[m_dof_index] = m_velocity;
		}

		//static data
		Boundary* Initial::m_boundary = nullptr;
	}
}
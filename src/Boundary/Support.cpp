//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"

#include "FEA/inc/Boundary/Support.hpp"
#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Solver.hpp"
#include "FEA/inc/Analysis/Analysis.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Support::Support(void) : m_node{0}, m_index{0}, m_dof_index{0}, m_dof{mesh::nodes::DOF::Translation_1}, m_reaction_data{nullptr}
		{
			return;
		}
		
		//destructor
		Support::~Support(void)
		{
			delete[] m_reaction_data;
		}

		//data
		uint32_t Support::index(void) const
		{
			return m_index;
		}

		Boundary* Support::boundary(void)
		{
			return m_boundary;
		}
		
		mesh::nodes::Node* Support::node(void) const
		{
			return m_boundary->model()->mesh()->node(m_node);
		}
		mesh::nodes::Node* Support::node(uint32_t node)
		{
			return m_boundary->model()->mesh()->node(m_node = node);
		}

		mesh::nodes::DOF Support::dof(void) const
		{
			return m_dof;
		}
		mesh::nodes::DOF Support::dof(mesh::nodes::DOF dof)
		{
			return m_dof = dof;
		}

		//analysis
		void Support::check(void)
		{
			if(m_node >= m_boundary->model()->mesh()->nodes().size())
			{
				throw std::runtime_error("Error: Support has out of range node!");
			}
		}
		void Support::setup(void)
		{
			//data
			const uint32_t ns = m_boundary->model()->analysis()->solver()->m_step_max;
			//setup
			delete[] m_reaction_data;
			m_reaction_data = new double[ns + 1];
		}
		void Support::record(void)
		{
			//data
			const uint32_t step = m_boundary->model()->analysis()->solver()->m_step;
			const double* R = m_boundary->model()->analysis()->solver()->reactions();
			//record
			m_reaction_data[step] = R[m_dof_index];
		}
		void Support::cleanup(void)
		{
			delete[] m_reaction_data;
		}
	}
}
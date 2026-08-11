//std
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Constraint::Constraint(void) : m_dof_index{0}
		{
			return;
		}

		//destructor
		Constraint::~Constraint(void)
		{
			return;
		}

		//serialization
		void Constraint::save(FILE* file) const
		{
			//header
			fprintf(file, "Index: %d Size: %zd ", m_index, m_dof.size());
			//nodes
			fprintf(file, "Nodes: ");
			for(uint32_t node : m_nodes) fprintf(file, "%d ", node);
			//dof
			fprintf(file, "DOF: ");
			for(mesh::nodes::DOF dof : m_dof) fprintf(file, "%d ", uint32_t(dof));

		}

		//data
		Constraint::Hessian Constraint::hessian(void) const
		{
			return m_hessian;
		}
		Constraint::Hessian Constraint::hessian(Hessian hessian)
		{
			return m_hessian = hessian;
		}

		Constraint::Gradient Constraint::gradient(void) const
		{
			return m_gradient;
		}
		Constraint::Gradient Constraint::gradient(Gradient gradient)
		{
			return m_gradient = gradient;
		}

		Constraint::Function Constraint::function(void) const
		{
			return m_function;
		}
		Constraint::Function Constraint::function(Function function)
		{
			return m_function = function;
		}

		const std::vector<uint32_t>& Constraint::nodes(void) const
		{
			return m_nodes;
		}
		const std::vector<mesh::nodes::DOF>& Constraint::dof(void) const
		{
			return m_dof;
		}

		//index
		uint32_t Constraint::index(void) const
		{
			return m_index;
		}

		//analysis
		void Constraint::check(void)
		{
			for(uint32_t node : m_nodes)
			{
				if(node >= m_boundary->model()->mesh()->nodes().size())
				{
					throw std::runtime_error("Error: Constraint " + std::to_string(m_index) + " has an out of range node!");
				}
			}
			if(m_nodes.size() != m_dof.size())
			{
				throw std::runtime_error("Error: Constraint " + std::to_string(m_index) + " nodes and dofs lists are incompatible!");
			}
			if(!m_function || !m_gradient || !m_hessian)
			{
				throw std::runtime_error("Error: Constraint " + std::to_string(m_index) + " functions are unset!");
			}
		}
		void Constraint::setup(void)
		{
			m_dof_indexes.clear();
			for(uint32_t i = 0; i < m_nodes.size(); i++)
			{
				m_dof_indexes.push_back(m_boundary->model()->mesh()->node(m_nodes[i])->dof_index(m_dof[i]));
			}
		}
		void Constraint::state(double* xc) const
		{
			//data
			const double t = m_boundary->model()->analysis()->solver()->time_new();
			const double* x = m_boundary->model()->analysis()->solver()->state_new();
			const uint32_t nu = m_boundary->model()->analysis()->assembler()->dof_unknow();
			//state
			for(uint32_t i = 0; i < m_dof_indexes.size(); i++)
			{
				xc[i] = m_dof_indexes[i] < nu ? x[m_dof_indexes[i]] : m_boundary->support(m_dof_indexes[i] - nu)->state(t);
			}
		}
		void Constraint::dof_setup(uint32_t& dof_counter)
		{
			m_dof_index = dof_counter++;
		}

		//static
		Boundary* Constraint::m_boundary = nullptr;
	}
}
//std
#include <algorithm>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Initials/Initial.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"
#include "FEA/inc/Boundary/Dependencies/Dependency.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace boundary
	{
		//constructors
		Dependency::Dependency(uint32_t n0, mesh::nodes::dof d0, uint32_t n1, mesh::nodes::dof d1) : 
			m_nodes{n0, n1}, m_dof{d0, d1}
		{
			return;
		}

		//destructor
		Dependency::~Dependency(void)
		{
			return;
		}

		//serialization
		void Dependency::load(FILE* file)
		{
			uint32_t d0, d1;
			if(fscanf(file, "%d %d %d %d", &m_nodes[0], &d0, &m_nodes[1], &d1) != 4)
			{
				throw std::runtime_error("Unable to load dependency data from file!");
			}
			m_dof[0] = mesh::nodes::dof(d0);
			m_dof[1] = mesh::nodes::dof(d1);
		}
		void Dependency::save(FILE* file) const
		{
			const uint32_t n0 = m_nodes[0];
			const uint32_t n1 = m_nodes[1];
			const uint32_t d0 = uint32_t(m_dof[0]);
			const uint32_t d1 = uint32_t(m_dof[1]);
			if(fprintf(file, "%d %d %d %d ", n0, d0, n1, d1) < 0)
			{
				throw std::runtime_error("Unable to save dependency data to file!");
			}
		}

		//data
		Boundary* Dependency::boundary(void)
		{
			return m_boundary;
		}

		mesh::nodes::Node* Dependency::node(uint32_t index) const
		{
			return m_boundary->model()->mesh()->node(m_nodes[index]);
		}
		mesh::nodes::Node* Dependency::node(uint32_t index, uint32_t node)
		{
			return m_boundary->model()->mesh()->node(m_nodes[index] = node);
		}

		mesh::nodes::dof Dependency::dof(uint32_t index) const
		{
			return m_dof[index];
		}
		mesh::nodes::dof Dependency::dof(uint32_t index, mesh::nodes::dof dof)
		{
			return m_dof[index] = dof;
		}

		//index
		uint32_t Dependency::index(void) const
		{
			return m_index;
		}
		uint32_t Dependency::index_node(uint32_t index) const
		{
			return m_nodes[index];
		}

		//check
		bool Dependency::check(void) const
		{
			return 
				check_inner() && check_outer() &&
				check_nodes() && check_initials() && check_supports();
		}
		bool Dependency::check_inner(void) const
		{
			if(m_nodes[0] == m_nodes[1] && m_dof[0] == m_dof[1])
			{
				printf("Dependency %02d is inner inconsistent!\n", m_index);
				return false;
			}
			return true;
		}
		bool Dependency::check_outer(void) const
		{
			for(const Dependency* dependency : m_boundary->dependencies())
			{
				if(dependency == this) continue;
				if(*this == *dependency) return false;
			}
			return true;
		}
		bool Dependency::check_nodes(void) const
		{
			//data
			const uint32_t nn = m_boundary->model()->mesh()->nodes().size();
			//check
			if(m_nodes[0] >= nn || m_nodes[1] >= nn)
			{
				printf("Dependency %02d has out of range nodes!\n", m_index);
				return false;
			}
			//return
			return true;
		}
		bool Dependency::check_initials(void) const
		{
			bool test[] = {true, true};
			for(uint32_t i = 0; i < 2; i++)
			{
				for(const Initial* initial : m_boundary->initials())
				{
					test[i] = test[i] && (m_nodes[i] != initial->index_node() || m_dof[i] != initial->dof());
				}
			}
			return test[0] || test[1];
		}
		bool Dependency::check_supports(void) const
		{
			bool test[] = {true, true};
			for(uint32_t i = 0; i < 2; i++)
			{
				for(const Support* support : m_boundary->supports())
				{
					test[i] = test[i] && (m_nodes[i] != support->index_node() || m_dof[i] != support->dof());
				}
			}
			return test[0] || test[1];
		}

		//dof
		uint32_t Dependency::dof_index(bool flag) const
		{
			return 
				m_nodes[0] < m_nodes[1] || (m_nodes[0] == m_nodes[1] && m_dof[0] < m_dof[1]) ? 
				node(flag)->dof_index(m_dof[flag]) : node(!flag)->dof_index(m_dof[!flag]);
		}

		//operators
		bool Dependency::operator==(const Dependency& dependency) const
		{
			const bool test[] = {
				m_nodes[0] == dependency.m_nodes[0] && m_dof[0] == dependency.m_dof[0] &&
				m_nodes[1] == dependency.m_nodes[1] && m_dof[1] == dependency.m_dof[1],
				m_nodes[0] == dependency.m_nodes[1] && m_dof[0] == dependency.m_dof[1] &&
				m_nodes[1] == dependency.m_nodes[0] && m_dof[1] == dependency.m_dof[0]
			};
			return test[0] && test[1];
		}

		//analysis
		void Dependency::setup(void)
		{
			return;
		}
		void Dependency::apply_dof(void) const
		{
			node(0)->apply_dof(m_dof[0]);
			node(1)->apply_dof(m_dof[1]);
		}

		//static data
		Boundary* Dependency::m_boundary = nullptr;
	}
}
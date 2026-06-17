//std
#include <cmath>
#include <stdexcept>

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Solvers/WatchDof.hpp"

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			//constructors
			WatchDof::WatchDof(Solver* solver) : 
				m_node(0), m_solver(solver), m_value_min(-INFINITY), m_value_max(+INFINITY), 
				m_dof(mesh::nodes::dof::translation_1)
			{
				return;
			}

			//destructor
			WatchDof::~WatchDof(void)
			{
				return;
			}

			//serialization
			void WatchDof::load(FILE* file)
			{
				uint32_t dof;
				if(fscanf(file, "%d %d %lf %lf", &m_node, &dof, &m_value_min, &m_value_max) != 4)
				{
					throw std::runtime_error("Watch dof loading error!");
				}
				m_dof = mesh::nodes::dof(dof);
			}
			void WatchDof::save(FILE* file) const
			{
				fprintf(file, "%d %d %+.6e %+.6e ", m_node, (uint32_t) m_dof, m_value_min, m_value_max);
			}

			//data
			double WatchDof::value(void) const
			{
				return m_solver->analysis()->model()->mesh()->node(m_node)->state(m_dof);
			}

			double WatchDof::value_min(void) const
			{
				return m_value_min;
			}
			double WatchDof::value_min(double value_min)
			{
				return m_value_min = value_min;
			}

			double WatchDof::value_max(void) const
			{
				return m_value_max;
			}
			double WatchDof::value_max(double value_max)
			{
				return m_value_max = value_max;
			}

			mesh::nodes::Node* WatchDof::node(void) const
			{
				return m_solver->analysis()->model()->mesh()->node(m_node);
			}
			mesh::nodes::Node* WatchDof::node(uint32_t node)
			{
				m_node = node;
				return m_solver->analysis()->model()->mesh()->node(m_node);
			}

			mesh::nodes::dof WatchDof::dof(void) const
			{
				return m_dof;
			}
			mesh::nodes::dof WatchDof::dof(mesh::nodes::dof dof)
			{
				return m_dof = dof;
			}

			//index
			uint32_t WatchDof::index_dof(void) const
			{
				return m_dof_index;
			}
			uint32_t WatchDof::index_node(void) const
			{
				return m_node;
			}

			//analysis
			void WatchDof::setup(void)
			{
				m_dof_index = m_solver->analysis()->model()->mesh()->node(m_node)->dof_index(m_dof);
			}
			bool WatchDof::checkup(void) const
			{
				if(m_value_min >= m_value_max)
				{
					printf("Watch dof has inconsistent dof limits!\n");
					return false;
				}
				if(m_node >= m_solver->analysis()->model()->mesh()->nodes().size())
				{
					printf("Watch dof has an out of range node index!\n");
					return false;
				}
				return true;
			}
		}
	}
}
//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Incremental.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Incremental::Incremental(void)
		{
			return;
		}
		
		//destructor
		Incremental::~Incremental(void)
		{
			return;
		}

		//save
		void Incremental::save(const char* path, std::vector<std::pair<uint32_t, mesh::nodes::DOF>> list) const
		{
			std::vector<uint32_t> dof_list;
			for(const std::pair<uint32_t, mesh::nodes::DOF>& item : list)
			{
				if(m_analysis->model()->mesh()->node(item.first)->dof_set() & 1 << uint32_t(item.second))
				{
					dof_list.push_back(m_analysis->model()->mesh()->node(item.first)->dof_index(item.second));
				}
			}
			math::solvers::Incremental::save(path, dof_list);
		}

		//analysis
		void Incremental::setup(void)
		{
			Solver::setup();
		}
	}
}
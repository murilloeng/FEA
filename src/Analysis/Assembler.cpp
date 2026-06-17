//math
#include "Math/inc/Miscellaneous/bits.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

#include "FEA/inc/Boundary/Support.hpp"
#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Assembler::Assembler(void)
		{
			return;
		}

		//destructor
		Assembler::~Assembler(void)
		{
			return;
		}

		//analysis
		void Assembler::setup(void)
		{
			apply_dof();
			count_dof();
			sort_dof();
		}
		void Assembler::sort_dof(void)
		{
			//data
			uint32_t counter_dof_known = 0;
			uint32_t counter_dof_unknown = 0;
			//nodes
			for(mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
			{
				
			}
		}
		void Assembler::apply_dof(void)
		{
			//setup
			for(mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
			{
				node->m_dof = 0;
			}
			//elements
			for(mesh::elements::Element* element : m_analysis->model()->mesh()->elements())
			{
				for(uint32_t i = 0; i < element->nodes().size(); i++)
				{
					element->node(i)->apply_dof(element->dof(i));
				}
			}
			//supports
			for(boundary::Support* support : m_analysis->model()->boundary()->supports())
			{
				support->node()->apply_dof(support->dof());
			}
		}
		void Assembler::count_dof(void)
		{
			//data
			m_dof_total = 0;
			m_dof_known = m_analysis->model()->boundary()->supports().size();
			//count
			for(const mesh::nodes::Node* node : m_analysis->model()->mesh()->nodes())
			{
				m_dof_total += math::bit_count(node->m_dof);
			}
			m_dof_unknown = m_dof_total - m_dof_known;
		}
	}
}
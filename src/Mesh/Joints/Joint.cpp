//FEA
#include "FEA/inc/Mesh/Joints/Joint.hpp"

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			//constructor
			Joint::Joint(void)
			{
				return;
			}

			//destructor
			Joint::~Joint(void)
			{
				return;
			}

			//serialization
			void Joint::save(FILE* file) const
			{
				fprintf(file, "Index: %d Nodes: %zd - ", m_index, m_nodes.size());
				for(uint32_t node : m_nodes) fprintf(file, "%d ", node);
			}

			//analysis
			void Joint::setup(void)
			{
				create_constraints();
				create_dependencies();
			}

			//static
			Mesh* Joint::m_mesh = nullptr;
		}
	}
}
//std
#include <stdexcept>

//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
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
			void Joint::check(void)
			{
				for(uint32_t node : m_nodes)
				{
					if(node >= m_mesh->nodes().size())
					{
						throw std::runtime_error("Error: Joint's node index is out of range!");
					}
				}
			}
			void Joint::setup(void)
			{
				return;
			}

			//static
			Mesh* Joint::m_mesh = nullptr;
		}
	}
}
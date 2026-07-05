//std
#include <stdexcept>

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Mechanic.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructor
			Mechanic::Mechanic(void) : m_material{nullptr}
			{
				return;
			}
			
			//destructor
			Mechanic::~Mechanic(void)
			{
				return;
			}

			//analysis
			void Mechanic::check(void)
			{
				if(!m_material)
				{
					throw std::runtime_error("Error: Mechanic element has unset material!");
				}
				Element::check();
			}
		}
	}
}

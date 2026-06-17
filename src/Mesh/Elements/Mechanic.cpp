//FEA
#include "FEA/inc/Mesh/Elements/Mechanic.hpp"

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

			//data
			const materials::Mechanic* Mechanic::material(void) const
			{
				return m_material;
			}
			const materials::Mechanic* Mechanic::material(const materials::Mechanic* material)
			{
				return m_material = material;
			}
		}
	}
}
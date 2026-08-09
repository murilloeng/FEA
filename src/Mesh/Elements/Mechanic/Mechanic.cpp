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

			//data
			uint32_t Mechanic::stress_set(void) const
			{
				return 0;
			}

			Mechanic::Formulation Mechanic::formulation(void)
			{
				return m_formulation;
			}
			Mechanic::Formulation Mechanic::formulation(Formulation formulation)
			{
				return m_formulation = formulation;
			}

			const materials::Mechanic* Mechanic::material(void) const
			{
				return m_material;
			}
			const materials::Mechanic* Mechanic::material(const materials::Mechanic* material)
			{
				return m_material = material;
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

			//static
			Mechanic::Formulation Mechanic::m_formulation = Mechanic::Formulation::Corotational;
		}
	}
}

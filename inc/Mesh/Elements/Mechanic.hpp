#pragma once

//Materials
#include "Materials/inc/Mechanic/Mechanic.hpp"

//FEA
#include "FEA/inc/Mesh/Elements/Element.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Mechanic : public Element
			{
			public:
				//constructor
				Mechanic(void);

				//destructor
				virtual ~Mechanic(void);

				//data
				const materials::Mechanic* material(void) const;
				const materials::Mechanic* material(const materials::Mechanic*);

			protected:
				//data
				const materials::Mechanic* m_material;
			};
		}
	}
}
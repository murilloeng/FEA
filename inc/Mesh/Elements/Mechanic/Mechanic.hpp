#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Element.hpp"

//Materials
#include "Materials/inc/Mechanic/Mechanic.hpp"

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
				~Mechanic(void);

				//analysis
				void check(void) override;

				//data
				const materials::Mechanic* m_material;
			};
		}
	}
}
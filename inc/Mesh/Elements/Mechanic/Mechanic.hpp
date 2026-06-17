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
			protected:
				//constructors
				Mechanic(void);

				//destructor
				virtual ~Mechanic(void) override;

				//data
				const materials::Mechanic* material(void) const;
				const materials::Mechanic* material(const materials::Mechanic*);

			protected:
				//analysis
				void check(void) override;

				//data
				const materials::Mechanic* m_material;
			};
		}
	}
}
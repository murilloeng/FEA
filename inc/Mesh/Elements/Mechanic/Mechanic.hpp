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
				//constructor
				Mechanic(void);

				//destructor
				~Mechanic(void);

			public:
				//enum
				enum class Formulation
				{
					Corotational, Lagragian
				};

				//data
				virtual uint32_t stress_set(void) const;

				static Formulation formulation(void);
				static Formulation formulation(Formulation);

				const materials::Mechanic* material(void) const;
				const materials::Mechanic* material(const materials::Mechanic*);

			protected:
				//analysis
				void check(void) override;

				//data
				static Formulation m_formulation;
				const materials::Mechanic* m_material;
			};
		}
	}
}
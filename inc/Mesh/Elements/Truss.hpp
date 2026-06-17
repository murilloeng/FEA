#pragma once

//Sections
#include "Sections/inc/Section.hpp"

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
			class Truss : public Element
			{
			public:
				//constructor
				Truss(void);

				//destructor
				~Truss(void);

				//compute
				void compute(void) override;

			protected:
				//strains
				static double strain_measure(double);
				static double strain_hessian(double);
				static double strain_gradient(double);

				//data
				double m_f;
				double m_K;
				const sections::Section* m_section;
				const materials::Mechanic* m_material;
			};
		}
	}
}
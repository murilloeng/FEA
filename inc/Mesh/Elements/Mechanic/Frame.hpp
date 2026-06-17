#pragma once

//Sections
#include "Sections/inc/Section.hpp"

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Mechanic.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Frame : public Mechanic
			{
			protected:
				//constructors
				Frame(void);

				//destructor
				virtual ~Frame(void) override;

				//data
				const sections::Section* section(void) const;
				const sections::Section* section(const sections::Section*);

			public:
				//analysis
				void check(void) override;

			protected:
				//data
				const sections::Section* m_section;
			};
		}
	}
}
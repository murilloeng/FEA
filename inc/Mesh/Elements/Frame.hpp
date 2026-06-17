#pragma once

//Sections
#include "Sections/inc/Section.hpp"

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Frame : public Mechanic
			{
			public:
				//constructor
				Frame(void);

				//destructor
				~Frame(void);

				//data
				const sections::Section* section(void) const;
				const sections::Section* section(const sections::Section*);

			protected:
				//analysis
				void check(void) override;

				//data
				const sections::Section* m_section;
			};
		}
	}
}
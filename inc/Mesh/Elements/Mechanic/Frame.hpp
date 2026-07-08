#pragma once

//sections
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
				//constructor
				Frame(void);

				//destructor
				~Frame(void);

			public:
				//data
				const sections::Section* section(void) const;
				const sections::Section* section(sections::Section*);

			protected:
				//analysis
				void check(void) override;

				//data
				const sections::Section* m_section;
			};
		}
	}
}
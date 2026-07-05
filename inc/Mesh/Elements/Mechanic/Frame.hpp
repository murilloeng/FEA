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
			public:
				//constructor
				Frame(void);

				//destructor
				~Frame(void);

				//analysis
				void check(void) override;

				//data
				sections::Section* m_section;
			};
		}
	}
}
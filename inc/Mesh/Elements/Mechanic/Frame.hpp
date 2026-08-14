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
				const double* major_axis(void) const;
				const double* major_axis(const double*);
				const double* major_axis(double, double, double);

				const sections::Section* section(void) const;
				const sections::Section* section(sections::Section*);

			protected:
				//analysis
				void check(void) override;
				void setup(void) override;

				//draw
				void draw_setup(draw::Data&) const override;
				void draw_update(draw::Data&) const override;

				//data
				double m_Lr;
				double m_major_axis[3];
				const sections::Section* m_section;
			};
		}
	}
}
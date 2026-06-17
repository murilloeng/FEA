#pragma once

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Mechanic.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Line : public Mechanic
			{
			protected:
				//constructors
				Line(LoadCase*);

				//destructor
				virtual ~Line(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				uint32_t edge(uint32_t);
				uint32_t edge(void) const;

			protected:
				//data
				uint32_t m_edge;
			};
		}
	}
}
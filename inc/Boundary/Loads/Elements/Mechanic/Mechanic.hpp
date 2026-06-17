#pragma once

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Element.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Mechanic : public Element
			{
			protected:
				//constructors
				Mechanic(LoadCase*);

				//destructors
				virtual ~Mechanic(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				double direction(uint32_t) const;
				const double* direction(void) const;
				const double* direction(const double*);
				const double* direction(double, double, double);

			protected:
				//data
				double m_direction[3];
			};
		}
	}
}
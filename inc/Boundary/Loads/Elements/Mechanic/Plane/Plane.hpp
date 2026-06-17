#pragma once

//FEA
#include "FEA/inc/Boundary/Loads/Elements/Mechanic/Mechanic.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Plane : public Mechanic
			{
			protected:
				//constructors
				Plane(LoadCase*);

				//destructor
				virtual ~Plane(void) override;

				//serialization
				void load(FILE*) override;
				void save(FILE*) const override;

			public:
				//data
				uint32_t face(uint32_t);
				uint32_t face(void) const;

			protected:
				//data
				uint32_t m_face;
			};
		}
	}
}
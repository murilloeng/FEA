#pragma once

//std
#include <vector>
#include <cstdint>

//FEA
#include "FEA/inc/Boundary/Loads/Load.hpp"

namespace fea
{
	namespace boundary
	{
		class LoadCase;
	}
}

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			class Element : public Load
			{
			protected:
				//constructor
				Element(void);

				//destructor
				virtual ~Element(void);

				//serialization
				void save(FILE*) const override;

			public:
				//data
				uint32_t element(uint32_t) const;
				const std::vector<uint32_t>& elements(void) const;

			protected:
				//data
				std::vector<uint32_t> m_elements;

				//friends
				friend class fea::boundary::LoadCase;
			};
		}
	}
}
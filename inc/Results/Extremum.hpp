#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace results
	{
		struct Extremum
		{
			uint32_t m_step;
			uint32_t m_type;
			double m_data[2];
		};
	}
}
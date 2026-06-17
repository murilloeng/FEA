#pragma once

//std
#include <cfloat>
#include <cstdint>

namespace fea
{
	namespace results
	{
		struct Dof
		{
			uint32_t m_step[2] = {0, 0};
			uint32_t m_node[2] = {0, 0};
			double m_value[2] = {+DBL_MAX, -DBL_MAX};
		};
	}
}
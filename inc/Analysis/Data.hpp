#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		enum class Data : uint32_t
		{
			Load			= 1 << 0,
			Time			= 1 << 1,
			State			= 1 << 2,
			Velocity		= 1 << 3,
			Acceleration	= 1 << 4
		};
	}
}
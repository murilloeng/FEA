#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace boundary
	{
		namespace time
		{
			enum class Type : uint32_t
			{
				Custom,
				Linear,
				SineWave,
				Constant,
				Quadratic,
				Last
			};
		}
	}
}
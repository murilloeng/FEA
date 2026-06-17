#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			enum class Type : uint32_t
			{
				LineForce		= 1 << 0,
				LineMoment		= 1 << 1,
				PlaneForce		= 1 << 2,
				PlaneMoment		= 1 << 3,
				VolumeForce		= 1 << 4,
				last
			};
		}
	}
}
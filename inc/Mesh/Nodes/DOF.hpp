#pragma once

//std
#include <cstdint>

namespace fea
{
	enum class DOF : uint32_t
	{
		Rotation_1		= 1 << 3,
		Rotation_2		= 1 << 4,
		Rotation_3		= 1 << 5,
		Translation_1	= 1 << 0,
		Translation_2	= 1 << 1,
		Translation_3	= 1 << 2
	};
}
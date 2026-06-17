#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			enum class Type : uint32_t
			{
				Drift				= 1 << 0,
				Modal				= 1 << 1,
				State				= 1 << 2,
				Buckling			= 1 << 3,
				StaticLinear		= 1 << 4,
				DynamicLinear		= 1 << 5,
				TransientLinear		= 1 << 6,
				StaticNonlinear		= 1 << 7,
				DynamicNonlinear	= 1 << 8,
				TransientNonlinear	= 1 << 9,
				Last
			};
		}
	}
}
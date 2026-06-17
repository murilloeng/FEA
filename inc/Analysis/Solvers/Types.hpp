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
				Drift				= 1 <<  0,
				Modal				= 1 <<  1,
				State				= 1 <<  2,
				Buckling			= 1 <<  3,
				Tensegrity			= 1 <<  4,
				StaticLinear		= 1 <<  5,
				DynamicLinear		= 1 <<  6,
				TransientLinear		= 1 <<  7,
				StaticNonlinear		= 1 <<  8,
				DynamicNonlinear	= 1 <<  9,
				TransientNonlinear	= 1 << 10,
				last
			};
		}
	}
}
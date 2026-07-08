#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		enum class Type : uint32_t
		{
			StaticLinear, StaticNonlinear,
			DynamicLinear, DynamicNonlinear,
			TransientLinear, TransientNonlinear
		};
	}
}
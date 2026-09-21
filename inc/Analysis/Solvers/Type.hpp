#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		enum class Type : uint32_t
		{
			TangentDrift,
			StaticLinear, StaticNonlinear,
			DynamicLinear, DynamicNonlinear,
			TransientLinear, TransientNonlinear
		};
	}
}
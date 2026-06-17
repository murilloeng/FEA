#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			enum class field : uint32_t
			{
				force,
				stiffness
			};
		}
	}
}
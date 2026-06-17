#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			enum class Integration : uint32_t
			{
				newmark,
				runge_kutta
			};
		}
	}
}
#pragma once

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			enum class spectre : uint32_t
			{
				min,
				max,
				full,
				partial
			};
		}
	}
}
#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			enum class StrainMeasure : uint32_t
			{
				Linear,
				Quadratic,
				Logarithmic
			};
		}
	}
}
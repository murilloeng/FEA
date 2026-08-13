#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace draw
	{
		enum class Mode : uint32_t
		{
			Geometry, Mesh, ResultsNodes, ResultsElements
		};
	}
}
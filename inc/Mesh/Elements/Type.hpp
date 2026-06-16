#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			enum class Type : uint32_t
			{
				Truss2D,
				Truss3D
			};
		}
	}
}
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
				Truss2D	= 1 << 0,
				Truss3D	= 1 << 1,
				Last
			};
		}
	}
}
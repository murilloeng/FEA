#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			enum class Type : uint32_t
			{
				Hinge			= 1 << 0,
				Rigid2D			= 1 << 1,
				Rigid3D			= 1 << 2,
				Spherical		= 1 << 3,
				Revolute2D		= 1 << 4,
				Revolute3D		= 1 << 5,
				last
			};
		}
	}
}
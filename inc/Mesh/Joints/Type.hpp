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
				Rigid2D, Rigid3D, Revolute2D, Revolute3D, Last
			};
		}
	}
}
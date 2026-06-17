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
				//0D
				Mass		= 1 << 0,
				Spring		= 1 << 1,
				Damper		= 1 << 2,
				Inertia		= 1 << 3,
				//1D
				Bar2D		= 1 << 4,
				Bar3D		= 1 << 5,
				Beam2C		= 1 << 6,
				Beam2T		= 1 << 7,
				Beam3D		= 1 << 8,
				//2D
				Plane		= 1 << 9,
				Plate		= 1 << 10,
				Shell		= 1 << 11,
				Membrane	= 1 << 12,
				//3D
				Solid		= 1 << 13,
				//last
				last
			};
		}
	}
}
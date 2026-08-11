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
				Nodal, Beam2D, Beam3D, Truss2D, Truss3D, Last
			};
		}
	}
}
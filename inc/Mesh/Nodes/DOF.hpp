#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			enum class DOF : uint32_t
			{
				Translation_1, Translation_2, Translation_3, 
				Rotation_1, Rotation_2, Rotation_3, Temperature, Last
			};
		}
	}
}
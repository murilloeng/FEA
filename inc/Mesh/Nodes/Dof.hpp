#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			enum class dof : uint32_t
			{
				rotation_1		= 1 <<  3,
				rotation_2		= 1 <<  4,
				rotation_3		= 1 <<  5,
				temperature		= 1 <<  6,
				translation_1	= 1 <<  0,
				translation_2	= 1 <<  1,
				translation_3	= 1 <<  2,
				last
			};
		}
	}
}
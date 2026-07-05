#pragma once

//std
#include <vector>

namespace fea
{
	namespace boundary
	{
		class Boundary;
		namespace loads
		{
			class Load;
		}
	}
}

namespace fea
{
	namespace boundary
	{
		class LoadCase
		{
		public:
			//constructor
			LoadCase(void);

			//destructor
			~LoadCase(void);

			//data
			static Boundary* m_boundary;
			std::vector<loads::Load*> m_loads;
		};
	}
}
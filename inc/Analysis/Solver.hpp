#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		class Solver
		{
		public:
			//constructor
			Solver(void);

			//destructor
			~Solver(void);

			//data
			virtual uint32_t data(void) const = 0;
		};
	}
}
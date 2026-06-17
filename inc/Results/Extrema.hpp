#pragma once

//std
#include <vector>
#include <cstdint>

//FEA
#include "FEA/inc/Results/Extremum.hpp"

namespace fea
{
	namespace results
	{
		class Extrema
		{
		public:
			//constructors
			Extrema(uint32_t, const double**);

			//destructor
			~Extrema(void);

			//data
			const std::vector<Extremum>& points(uint32_t) const;

		private:
			//search
			void search(uint32_t);

			//data
			const uint32_t m_size;
			const double* m_data[2];
			std::vector<Extremum> m_points[2];
		};
	}
}
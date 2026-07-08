#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace boundary
	{
		class Boundary;
		class LoadCombination;
	}
}


namespace fea
{
	namespace boundary
	{
		class LoadItem
		{
		private:
			//constructor
			LoadItem(uint32_t, double, bool);

			//destructor
			~LoadItem(void);

		public:
			//data
			bool fixed(bool);
			bool fixed(void) const;

			double value(double);
			double value(void) const;

			uint32_t load_case(uint32_t);
			uint32_t load_case(void) const;

		private:
			//data
			bool m_fixed;
			double m_value;
			uint32_t m_load_case;

			//friends
			friend class fea::boundary::Boundary;
			friend class fea::boundary::LoadCombination;
		};
	}
}
#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		class Analysis;
	}
}

namespace fea
{
	namespace analysis
	{
		class Assembler
		{
		public:
			//constructor
			Assembler(void);

			//destructor
			~Assembler(void);

		private:
			//analysis
			void setup(void);
			void sort_dof(void);
			void apply_dof(void);
			void count_dof(void);

			//friends
			friend class Analysis;

			//data
			uint32_t m_dof_total;
			uint32_t m_dof_known;
			uint32_t m_dof_unknown;
			static Analysis* m_analysis;
		};
	}
}
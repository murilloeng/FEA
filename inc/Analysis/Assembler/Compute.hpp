#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		class Assembler;
	}
}

namespace fea
{
	namespace analysis
	{
		class Compute
		{
		private:
			//constructor
			Compute(void);
		
			//destructor
			~Compute(void);
		
			//setup
			void cleanup(void);
			void setup(uint32_t);
		
		public:
			//data
			double m_T, m_U, m_V;
			double *m_x, *m_fi, *m_fe, *m_K, *m_C, *m_M;

			//friends
			friend class fea::analysis::Assembler;
		};
	}
}
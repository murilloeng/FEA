#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		class Request;
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

		public:
			//setup
			void reset(void);
			void cleanup(void);
			void setup(uint32_t);

			//data
			uint32_t m_size;
			const Request* m_request;
			double m_T, m_U, m_V, *m_x, *m_f, *m_K, *m_C, *m_M;

			//friends
			friend class fea::analysis::Assembler;
		};
	}
}
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
		class Data
		{
		private:
			//constructor
			Data(void);
		
			//destructor
			~Data(void);

		public:
			//setup
			void reset(void);
			void cleanup(void);
			void setup(uint32_t);

			//data
			uint32_t m_size;
			const Request* m_request;
			double m_T, m_U, m_V, *m_x, *m_v, *m_a, *m_f, *m_K, *m_C, *m_M, *m_A;

			//friends
			friend class fea::analysis::Assembler;
		};
	}
}
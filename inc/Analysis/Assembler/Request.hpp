#pragma once

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
		class Request
		{
		private:
			//constructor
			Request(void);

			//destructor
			~Request(void) = default;

			//setup
			void setup(void);

		public:
			//data
			bool m_fi, m_ft;
			bool m_T, m_U, m_V;
			bool m_K, m_C, m_M;

			//friends
			friend class fea::analysis::Assembler;
		};
	}
}
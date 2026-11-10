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

		public:
			//data
			bool m_T, m_U, m_V;
			bool m_fi, m_fe, m_K, m_C, m_M;

			//friends
			friend class fea::analysis::Assembler;
		};
	}
}
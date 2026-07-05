#pragma once

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

			//data
			static Analysis* m_analysis;
		};
	}
}
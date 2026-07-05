#pragma once

namespace fea
{
	class Model;
	namespace analysis
	{
		class Solver;
		class Assembler;
	}
}

namespace fea
{
	namespace analysis
	{
		class Analysis
		{
		public:
			//constructor
			Analysis(void);

			//destructor
			~Analysis(void);

			//data
			Solver* m_solver;
			static Model* m_model;
			Assembler* m_assembler;
		};
	}
}
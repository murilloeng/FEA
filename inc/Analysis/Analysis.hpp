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

			//analysis
			void check(void);
			void setup(void);
			void dof_apply(void);

			//data
			Solver* m_solver;
			static Model* m_model;
			Assembler* m_assembler;
		};
	}
}
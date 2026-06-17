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
			static Model* model(void);

		private:
			//friends
			friend class fea::Model;

			//data
			Solver* m_solver;
			Assembler* m_assembler;

			static Model* m_model;
		};
	}
}
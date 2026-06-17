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
			Solver* solver(void) const;
			Assembler* assembler(void) const;

		private:
			//analysis
			void check(void);
			void setup(void);
			void record(void);
			void update(void);
			void restore(void);
			void compute(void);

			//friends
			friend class fea::Model;

			//data
			Solver* m_solver;
			Assembler* m_assembler;

			static Model* m_model;
		};
	}
}
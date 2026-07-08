#pragma once

//std
#include <cstdint>

namespace fea
{
	class Model;
	namespace analysis
	{
		class Solver;
		class Assembler;
		enum class Type : uint32_t;
	}
}

namespace fea
{
	namespace analysis
	{
		class Analysis
		{
		private:
			//constructor
			Analysis(void);
			
			//destructor
			~Analysis(void);
			
		public:
			//data
			void create_solver(Type);

			static Model* model(void);

			Solver* solver(void) const;
			Assembler* assembler(void) const;

		private:
			//analysis
			void check(void);
			void setup(void);
			void dof_apply(void);

			//data
			Solver* m_solver;
			static Model* m_model;
			Assembler* m_assembler;

			//friends
			friend class fea::Model;
			friend class fea::analysis::Solver;
			friend class fea::analysis::Assembler;
		};
	}
}
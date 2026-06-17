#pragma once

//std
#include <cstdio>
#include <cstdint>

namespace fea
{
	class Model;
	namespace analysis
	{
		class Assembler;
		namespace solvers
		{
			class Solver;
			enum class Type : uint32_t;
		}
	}
}

namespace fea
{
	namespace analysis
	{
		class Analysis
		{
		private:
			//constructors
			Analysis(void);

			//destructor
			~Analysis(void);

			//serialization
			void load(FILE*);
			void save(FILE*) const;

			void load_results(void);
			void save_results(void) const;

		public:
			//data
			bool silent(bool);
			bool silent(void) const;
			bool solved(void) const;
			static Model* model(void);
			Assembler* assembler(void) const;
			solvers::Solver* solver(void) const;
			solvers::Solver* solver(solvers::Type);

			//solve
			bool solve(bool = false);

		private:
			//analysis
			bool check(void) const;
			void setup(void) const;
			void update(void) const;
			void restore(void) const;
			void apply_dof(void) const;

			//data
			bool m_silent;
			bool m_solved;
			static Model* m_model;
			Assembler* m_assembler;
			solvers::Solver* m_solver;

			//friends
			friend class fea::Model;
			friend class fea::analysis::Assembler;
		};
	}
}
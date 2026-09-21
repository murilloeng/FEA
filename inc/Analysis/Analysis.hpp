#pragma once

//std
#include <cstdio>
#include <cstdint>

namespace fea
{
	class Model;
	namespace analysis
	{
		class Solver;
		class Assembler;
		class TangentDrift;
		class StaticLinear;
		class StaticNonlinear;
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

			//serialization
			void save(FILE*) const;

		public:
			//data
			Type type(Type);
			Type type(void) const;

			static Model* model(void);

			Solver* solver(void) const;
			Assembler* assembler(void) const;

			TangentDrift* solver_tangent_drift(void) const;
			StaticLinear* solver_static_linear(void) const;
			StaticNonlinear* solver_static_nonlinear(void) const;

		private:
			//analysis
			void check(void);
			void setup(void);
			void dof_apply(void);

			//data
			Type m_type;
			static Model* m_model;
			Assembler* m_assembler;
			TangentDrift* m_solver_tangent_drift;
			StaticLinear* m_solver_static_linear;
			StaticNonlinear* m_solver_static_nonlinear;

			//friends
			friend class fea::Model;
			friend class fea::analysis::Solver;
			friend class fea::analysis::Assembler;
		};
	}
}
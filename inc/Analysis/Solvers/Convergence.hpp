#pragma once

//std
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Nonlinear;
		}
	}
}

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class Convergence
			{
			private:
				//constructor
				Convergence(Nonlinear*);

				//destructor
				~Convergence(void);

				//serialization
				void load(FILE*);
				void save(FILE*) const;

			public:
				//types
				enum class Type : uint32_t
				{
					LoadFixed,
					LoadRelative,
					StateIncrement,
					Last
				};

				//compute
				bool compute(void);

				//data
				Type type(Type);
				Type type(void) const;

				bool status(void) const;

				double tolerance(double);
				double tolerance(void) const;

				//name
				static const char* type_name(Type);

			private:
				//data
				Type m_type;
				bool m_status;
				double m_tolerance;
				const Nonlinear* m_solver;

				//friends
				friend class Nonlinear;
			};
		}
	}
}
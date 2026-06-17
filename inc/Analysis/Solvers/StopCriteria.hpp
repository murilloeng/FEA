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
			class Solver;
		}
	}
}

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class StopCriteria
			{
			private:
				//constructor
				StopCriteria(Solver*);

				//destructor
				~StopCriteria(void);

				//serialization
				void load(FILE*);
				void save(FILE*) const;

			public:
				//types
				enum class Type : uint32_t
				{
					LoadMinimum		= 1 << 0,
					LoadMaximum		= 1 << 1,
					LoadNegative	= 1 << 2,
					LoadPositive	= 1 << 3,
					StateMinimum	= 1 << 4,
					StateMaximum	= 1 << 5,
					StateNegative	= 1 << 6,
					StatePositive	= 1 << 7,
					Last
				};

				//data
				uint32_t types(Type);
				uint32_t types(uint32_t);
				uint32_t types(void) const;

				//compute
				bool compute(void);

				//names
				static const char* type_name(Type);

			private:
				//compute
				bool compute_step(void) const;
				bool compute_attempt(void) const;
				bool compute_watch_dof(void) const;
				bool compute_load_limits(void) const;
				bool compute_time_limits(void) const;
				bool compute_load_minimum(void) const;
				bool compute_load_maximum(void) const;
				bool compute_load_negative(void) const;
				bool compute_load_positive(void) const;
				bool compute_state_minimum(void) const;
				bool compute_state_maximum(void) const;
				bool compute_state_negative(void) const;
				bool compute_state_positive(void) const;

				//data
				uint32_t m_types;
				Solver* m_solver;

				//friends
				friend class Solver;
			};
		}
	}
}
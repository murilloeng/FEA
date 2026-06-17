#pragma once

//std
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace models
	{
		class Model;
	}
	namespace results
	{
		class What;
		class Bounds;
	}
	namespace analysis
	{
		class Assembler;
	}
}

namespace fea
{
	namespace results
	{
		class Results
		{
		public:
			//constructors
			Results(void);

			//destructor
			~Results(void);

			//serialization
			void load(FILE*);
			void save(FILE*) const;

			//data
			bool status(void) const;
			uint32_t steps(void) const;
			uint32_t dof_set(void) const;
			uint32_t state_set(void) const;

			What* what(void) const;
			Bounds* bounds(void) const;
			static const models::Model* model(void);

		private:
			//setup
			void setup(void);

			//apply
			void apply_dof(void);
			void apply_state(void);

			//data
			bool m_status;
			uint32_t m_steps;
			uint32_t m_dof_set;
			uint64_t m_state_set;

			What* m_what;
			Bounds* m_bounds;
			static const models::Model* m_model;

			//friends
			friend class models::Model;
			friend class analysis::Assembler;
		};
	}
}
#pragma once

//std
#include <cstdint>

namespace fea
{
	class Model;
	namespace results
	{
		struct Dof;
		struct State;
		class Results;
	}
}

namespace fea
{
	namespace results
	{
		class Bounds
		{
		public:
			//constructors
			Bounds(void);

			//destructor
			~Bounds(void);

			//data
			const Results* results(void);
			const State& state(uint32_t) const;
			const Dof& dof(uint32_t, uint32_t) const;

		private:
			//setup
			void setup(void);

			//bound
			void bound_dof(void);
			void bound_state(void);

			//data
			Dof* m_dof[3];
			State* m_state;
			const results::Results* m_results;

			//friends
			friend class Results;
			friend class fea::Model;
		};
	}
}
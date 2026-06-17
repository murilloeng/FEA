#pragma once

//std
#include <cstdio>

namespace fea
{
	namespace results
	{
		class What
		{
		public:
			//constructors
			What(void);

			//destructor
			~What(void);

			//serialization
			void load(FILE*);
			void save(FILE*) const;

			//data
			bool solver(bool);
			bool states(bool);
			bool joints(bool);
			bool energies(bool);
			bool elements(bool);
			bool supports(bool);
			bool velocities(bool);
			bool accelerations(bool);

			bool solver(void) const;
			bool states(void) const;
			bool joints(void) const;
			bool energies(void) const;
			bool elements(void) const;
			bool supports(void) const;
			bool velocities(void) const;
			bool accelerations(void) const;

		private:
			//data
			bool m_state;
			bool m_joint;
			bool m_solver;
			bool m_energy;
			bool m_element;
			bool m_support;
			bool m_velocity;
			bool m_acceleration;
		};
	}
}
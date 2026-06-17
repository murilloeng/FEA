//std
#include <cstdlib>
#include <cstdint>
#include <stdexcept>

//FEA
#include "FEA/inc/Results/What.hpp"

namespace fea
{
	namespace results
	{
		What::What(void) : 
			m_state(true), 
			m_joint(true), 
			m_solver(true), 
			m_energy(true), 
			m_element(true), 
			m_support(true), 
			m_velocity(true), 
			m_acceleration(true)
		{
			return;
		}

		//destructor
		What::~What(void)
		{
			return;
		}

		//serialization
		void What::load(FILE* file)
		{
			uint32_t temp;
			bool* data[] = {
				&m_state, &m_velocity, &m_acceleration,
				&m_solver, &m_joint, &m_energy, &m_element, &m_support
			};
			for(bool* item : data)
			{
				if(fscanf(file, "%d", &temp) != 1)
				{
					throw std::runtime_error("What loading error!");
				}
				*item = temp;
			}
		}
		void What::save(FILE* file) const
		{
			const bool data[] = {
				m_state, m_velocity, m_acceleration,
				m_solver, m_joint, m_energy, m_element, m_support
			};
			for(const bool& item : data)
			{
				fprintf(file, "%d ", item);
			}
			fprintf(file, "\n");
		}

		//data
		bool What::solver(bool solver)
		{
			return m_solver = solver;
		}
		bool What::states(bool states)
		{
			return m_state = states;
		}
		bool What::joints(bool joints)
		{
			return m_joint = joints;
		}
		bool What::energies(bool energies)
		{
			return m_energy = energies;
		}
		bool What::elements(bool elements)
		{
			return m_element = elements;
		}
		bool What::supports(bool supports)
		{
			return m_support = supports;
		}
		bool What::velocities(bool velocities)
		{
			return m_velocity = velocities;
		}
		bool What::accelerations(bool accelerations)
		{
			return m_acceleration = accelerations;
		}

		bool What::solver(void) const
		{
			return m_solver;
		}
		bool What::states(void) const
		{
			return m_state;
		}
		bool What::joints(void) const
		{
			return m_joint;
		}
		bool What::energies(void) const
		{
			return m_energy;
		}
		bool What::elements(void) const
		{
			return m_element;
		}
		bool What::supports(void) const
		{
			return m_support;
		}
		bool What::velocities(void) const
		{
			return m_velocity;
		}
		bool What::accelerations(void) const
		{
			return m_acceleration;
		}
	}
}
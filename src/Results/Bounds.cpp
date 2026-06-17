//std
#include <cmath>
#include <cfloat>
#include <cstring>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/States.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

#include "FEA/inc/Results/Dof.hpp"
#include "FEA/inc/Results/State.hpp"
#include "FEA/inc/Results/Bounds.hpp"
#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

namespace fea
{
	namespace results
	{
		//constructors
		Bounds::Bounds(void) :
			m_dof{nullptr, nullptr, nullptr}, m_state{nullptr}
		{
			return;
		}

		//destructor
		Bounds::~Bounds(void)
		{
			delete[] m_state;
			delete[] m_dof[0];
			delete[] m_dof[1];
			delete[] m_dof[2];
		}

		//data
		const Results* Bounds::results(void)
		{
			return m_results;
		}

		const State& Bounds::state(uint32_t state_index) const
		{
			return m_state[state_index];
		}
		const Dof& Bounds::dof(uint32_t dof_index, uint32_t type_index) const
		{
			return m_dof[type_index][dof_index];
		}

		//setup
		void Bounds::setup(void)
		{
			//data
			const uint32_t dof_count = math::bit_count(m_results->dof_set());
			const uint32_t state_count = math::bit_count(m_results->state_set());
			const uint32_t ss = m_results->model()->analysis()->solver()->state_set();
			//allocate dof
			for(uint32_t i = 0; i < 3; i++)
			{
				if(1 << i & ss)
				{
					delete[] m_dof[i];
					m_dof[i] = new Dof[dof_count];
				}
			}
			//allocate state
			delete[] m_state;
			m_state = new State[state_count];
			//bound
			bound_dof();
			bound_state();
		}
		
		void Bounds::bound_dof(void)
		{
			//data
			const uint32_t ns = m_results->steps();
			const uint32_t nd = math::bit_count(m_results->dof_set());
			const uint32_t nn = m_results->model()->mesh()->nodes().size();
			const uint32_t ss = m_results->model()->analysis()->solver()->state_set();
			double (mesh::nodes::Node::*methods[])(mesh::nodes::dof, uint32_t) const = {
				&mesh::nodes::Node::state_data,
				&mesh::nodes::Node::velocity_data,
				&mesh::nodes::Node::acceleration_data
			};
			//bound
			for(uint32_t i = 0; i < 3; i++)
			{
				if(~ss & 1 << i) continue;
				for(uint32_t j = 0; j < nd; j++)
				{
					for(uint32_t k = 0; k < nn; k++)
					{
						const mesh::nodes::Node* node = m_results->model()->mesh()->node(k);
						const mesh::nodes::dof dof = mesh::nodes::dof(math::bit_search(m_results->dof_set(), j));
						if(uint32_t(dof) & node->dof_set())
						{
							for(uint32_t p = 0; p <= ns; p++)
							{
								if(m_dof[i][j].m_value[0] > (node->*methods[i])(dof, p))
								{
									m_dof[i][j].m_node[0] = k;
									m_dof[i][j].m_step[0] = p;
									m_dof[i][j].m_value[0] = (node->*methods[i])(dof, p);
								}
								if(m_dof[i][j].m_value[1] < (node->*methods[i])(dof, p))
								{
									m_dof[i][j].m_node[1] = k;
									m_dof[i][j].m_step[1] = p;
									m_dof[i][j].m_value[1] = (node->*methods[i])(dof, p);
								}
							}
						}
					}
				}
			}
		}
		void Bounds::bound_state(void)
		{
			//data
			const uint32_t ns = m_results->steps();
			const uint32_t nt = math::bit_count(m_results->state_set());
			const uint32_t ne = m_results->model()->mesh()->elements().size();
			//bound
			for(uint32_t i = 0; i < nt; i++)
			{
				for(uint32_t j = 0; j < ne; j++)
				{
					const uint32_t nn = m_results->model()->mesh()->element(j)->nodes().size();
					const mesh::elements::Element* element = m_results->model()->mesh()->element(j);
					const mesh::elements::State state = mesh::elements::State(math::bit_search(m_results->state_set(), i));
					if(uint64_t(state) & element->states_set())
					{
						for(uint32_t k = 0; k < nn; k++)
						{
							for(uint32_t p = 0; p <= ns; p++)
							{
								if(m_state[i].m_value[0] > element->state_data(state, k, p))
								{
									m_state[i].m_node[0] = k;
									m_state[i].m_step[0] = p;
									m_state[i].m_element[0] = j;
									m_state[i].m_value[0] = element->state_data(state, k, p);
								}
								if(m_state[i].m_value[1] < element->state_data(state, k, p))
								{
									m_state[i].m_node[1] = k;
									m_state[i].m_step[1] = p;
									m_state[i].m_element[1] = j;
									m_state[i].m_value[1] = element->state_data(state, k, p);
								}
							}
						}
					}
				}
			}
		}
	}
}
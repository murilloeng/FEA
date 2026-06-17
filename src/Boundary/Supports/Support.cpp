//std
#include <cmath>
#include <cstring>
#include <algorithm>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Results/What.hpp"
#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Time/Time.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

//static data
static const uint32_t dof_r = 
	uint32_t(fea::mesh::nodes::dof::rotation_1) |
	uint32_t(fea::mesh::nodes::dof::rotation_2) |
	uint32_t(fea::mesh::nodes::dof::rotation_3);

namespace fea
{
	namespace boundary
	{
		//construtors
		Support::Support(void) :
			m_node(0), m_dof(mesh::nodes::dof::translation_1), 
			m_state(UINT_MAX), m_velocity(UINT_MAX), m_acceleration(UINT_MAX), m_draw_direction(true), m_reactions_data(nullptr)
		{
			return;
		}

		//destructor
		Support::~Support(void)
		{
			delete[] m_reactions_data;
		}

		//serialization
		void Support::load(FILE* file)
		{
			uint32_t dof;
			const char* format = "%d %d %d %d %d";
			if(fscanf(file, format, &m_node, &dof, &m_state, &m_velocity, &m_acceleration) != 5)
			{
				throw std::runtime_error("Unable to read Support data from file!");
			}
			m_dof = mesh::nodes::dof(dof);
		}
		void Support::save(FILE* file) const
		{
			const char* format = "%d %d %d %d %d";
			if(fprintf(file, format, m_node, (uint32_t) m_dof, m_state, m_velocity, m_acceleration) < 0)
			{
				throw std::runtime_error("Unable to write Support data to file!");
			}
		}

		void Support::load_state(void)
		{
			//data
			char buffer[800];
			const uint32_t ns = m_boundary->model()->results()->steps();
			sprintf(buffer, "%s/Supports/S%04d.txt", m_boundary->model()->folder().c_str(), m_index);
			//check
			if(!m_boundary->model()->results()->what()->supports()) return;
			//allocate
			delete[] m_reactions_data;
			m_reactions_data = new double[ns + 1];
			//open
			FILE* file = fopen(buffer, "r");
			//load
			if(!file)
			{
				sprintf(buffer, "Unable to open support %04d results file!", m_index);
				throw std::runtime_error(buffer);
			}
			for(uint32_t i = 0; i <= ns; i++)
			{
				if(fscanf(file, "%lf", &m_reactions_data[i]) != 1)
				{
					sprintf(buffer, "Unable to read from support %04d results file!", m_index);
					throw std::runtime_error(buffer);
				}
			}
			//close
			fclose(file);
		}
		void Support::save_state(void) const
		{
			//data
			char buffer[800];
			const uint32_t ns = m_boundary->model()->results()->steps();
			sprintf(buffer, "%s/Supports/S%04d.txt", m_boundary->model()->folder().c_str(), m_index);
			//check
			if(!m_boundary->model()->results()->what()->supports()) return;
			//open
			FILE* file = fopen(buffer, "w");
			//save
			if(!file)
			{
				sprintf(buffer, "Unable to create support %04d results file!", m_index);
				throw std::runtime_error(buffer);
			}
			for(uint32_t i = 0; i <= ns; i++)
			{
				if(fprintf(file, "%+.6e\n", m_reactions_data[i]) < 0)
				{
					fclose(file);
					sprintf(buffer, "Unable to write to support %04d results file!", m_index);
					throw std::runtime_error(buffer);
				}
			}
			//close
			fclose(file);
		}

		//data
		bool Support::draw_direction(void) const
		{
			return m_draw_direction;
		}
		bool Support::draw_direction(bool draw_direction)
		{
			return m_draw_direction = draw_direction;
		}

		Boundary* Support::boundary(void)
		{
			return m_boundary;
		}

		mesh::nodes::dof Support::dof(void) const
		{
			return m_dof;
		}
		mesh::nodes::dof Support::dof(mesh::nodes::dof dof)
		{
			return m_dof = dof;
		}

		mesh::nodes::Node* Support::node(void) const
		{
			return m_boundary->model()->mesh()->node(m_node);
		}
		mesh::nodes::Node* Support::node(uint32_t node)
		{
			if(node >= m_boundary->model()->mesh()->nodes().size())
			{
				throw std::runtime_error("Support's node index is out of range!");
			}
			return m_boundary->model()->mesh()->node(m_node = node);
		}

		time::Time* Support::state(uint32_t state)
		{
			if(state != UINT_MAX && state >= m_boundary->times().size())
			{
				throw std::runtime_error("Support's state index is out of range!");
			}
			m_state = state;
			return m_state == UINT_MAX ? nullptr : m_boundary->time(m_state);
		}
		time::Time* Support::velocity(uint32_t velocity)
		{
			if(velocity != UINT_MAX && velocity >= m_boundary->times().size())
			{
				throw std::runtime_error("Support's velocity index is out of range!");
			}
			m_velocity = velocity;
			return m_velocity == UINT_MAX ? nullptr : m_boundary->time(m_velocity);
		}
		time::Time* Support::acceleration(uint32_t acceleration)
		{
			if(acceleration != UINT_MAX && acceleration >= m_boundary->times().size())
			{
				throw std::runtime_error("Support's acceleration index is out of range!");
			}
			m_acceleration = acceleration;
			return m_acceleration == UINT_MAX ? nullptr : m_boundary->time(m_acceleration);
		}

		time::Time* Support::state(void) const
		{
			return m_state == UINT_MAX ? nullptr : m_boundary->time(m_state);
		}
		time::Time* Support::velocity(void) const
		{
			return m_velocity == UINT_MAX ? nullptr : m_boundary->time(m_velocity);
		}
		time::Time* Support::acceleration(void) const
		{
			return m_acceleration == UINT_MAX ? nullptr : m_boundary->time(m_acceleration);
		}

		//index
		uint32_t Support::index(void) const
		{
			return m_index;
		}
		uint32_t Support::index_node(void) const
		{
			return m_node;
		}
		uint32_t Support::index_state(void) const
		{
			return m_state;
		}
		uint32_t Support::index_velocity(void) const
		{
			return m_velocity;
		}
		uint32_t Support::index_acceleration(void) const
		{
			return m_acceleration;
		}

		//data
		double Support::dof_state(double t) const
		{
			return m_state == UINT_MAX ? 0 : state()->value(t);
		}
		double Support::dof_velocity(double t) const
		{
			return m_velocity == UINT_MAX ? 0 : velocity()->value(t);
		}
		double Support::dof_acceleration(double t) const
		{
			return m_acceleration == UINT_MAX ? 0 : acceleration()->value(t);
		}

		//results
		double Support::state_data(uint32_t step) const
		{
			return m_reactions_data ? m_reactions_data[step] : 0;
		}

		//analysis
		void Support::record(void)
		{
			//data
			const double* R = m_boundary->model()->analysis()->solver()->m_R;
			const uint32_t st = m_boundary->model()->analysis()->solver()->step();
			const uint32_t nu = m_boundary->model()->analysis()->assembler()->dof_unknow();
			//check
			if(!m_boundary->model()->results()->what()->supports()) return;
			//reaction
			m_reactions_data[st] = R[m_dof_index - nu];
		}
		void Support::setup(void)
		{
			//data
			const uint32_t ns = m_boundary->model()->analysis()->solver()->step_max();
			//rotation
			const uint32_t dof_rotation =
				(uint32_t) mesh::nodes::dof::rotation_1 |
				(uint32_t) mesh::nodes::dof::rotation_2 |
				(uint32_t) mesh::nodes::dof::rotation_3;
			if(uint32_t(m_dof) & dof_rotation && (node()->m_dof_set & dof_rotation) == dof_rotation)
			{
				Support::m_update_nodes.push_back(m_node);
			}
			//data
			delete[] m_reactions_data;
			m_reactions_data = new double[ns + 1];
			//dof index
			mesh::nodes::Node* node = m_boundary->model()->mesh()->node(m_node);
			m_dof_index = node->m_dof_index[math::bit_index(node->m_dof_set, (uint32_t) m_dof)];
		}
		void Support::apply(void) const
		{
			//data
			const double t = m_boundary->model()->analysis()->solver()->m_t_new;
			const uint32_t s = m_boundary->model()->analysis()->solver()->state_set();
			const uint8_t p = math::bit_index(node()->m_dof_set, (uint32_t) m_dof);
			//apply
			if(s & (uint32_t) analysis::solvers::state::x) node()->m_state_new[p] = dof_state(t);
			if(s & (uint32_t) analysis::solvers::state::v) node()->m_velocity_new[p] = dof_velocity(t);
			if(s & (uint32_t) analysis::solvers::state::a) node()->m_acceleration_new[p] = dof_acceleration(t);
		}
		bool Support::check(void) const
		{
			for(Support* support : m_boundary->supports())
			{
				if(support != this)
				{
					if(m_node == support->m_node && m_dof == support->m_dof)
					{
						const char msg[] = "Support %02d (Node: %04d dof: %s) is incompatible with support %02d!\n";
						printf(msg, m_index, m_node, mesh::nodes::Node::dof_name(m_dof), support->m_index);
						return false;
					}
				}
			}
			return true;
		}
		void Support::apply_dof(void) const
		{
			m_boundary->model()->mesh()->node(m_node)->apply_dof(m_dof);
		}

		//static data
		Boundary* Support::m_boundary = nullptr;
		std::list<uint32_t> Support::m_update_nodes;
	}
}
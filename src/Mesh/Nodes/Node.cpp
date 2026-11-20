//std
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Groups/ASO3.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

//math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Solvers/Incremental.hpp"

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			//constructor
			Node::Node(const double* x) : m_dof_set{0}, m_position{x[0], x[1], x[2]}, m_rotation{nullptr}
			{
				return;
			}
			Node::Node(double x1, double x2, double x3) : m_dof_set{0}, m_position{x1, x2, x3}, m_rotation{nullptr}
			{
				return;
			}

			//destructor
			Node::~Node(void)
			{
				delete m_rotation;
			}

			//serialization
			void Node::save(FILE* file) const
			{
				fprintf(file, "Index: %d Position: %+.6e %+.6e %+.6e", m_index, m_position[0], m_position[1], m_position[2]);
			}

			//data
			uint32_t Node::index(void) const
			{
				return m_index;
			}
			uint32_t Node::dof_set(void) const
			{
				return m_dof_set;
			}
			uint32_t Node::dof_index(DOF dof) const
			{
				if(~m_dof_set & 1 << uint32_t(dof))
				{
					throw std::runtime_error("Error: Node's dof index called with unset dof!");
				}
				return m_dof_indexes[math::bit_index(m_dof_set, 1 << uint32_t(dof))];
			}

			double Node::state(DOF dof) const
			{
				//data
				const uint8_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const double t = m_mesh->model()->analysis()->solver()->time_new();
				const double* x = m_mesh->model()->analysis()->solver()->state_new();
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				//return
				return 
					(~m_dof_set & 1 << uint32_t(dof)) || !x ? 0 :
					m_dof_indexes[id] < nu ? x[m_dof_indexes[id]] : 
					m_mesh->model()->boundary()->support(m_dof_indexes[id] - nu)->state(t);
			}
			double Node::velocity(DOF dof) const
			{
				//data
				const uint8_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const double t = m_mesh->model()->analysis()->solver()->time_new();
				const double* v = m_mesh->model()->analysis()->solver()->velocity_new();
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				//return
				return 
					(~m_dof_set & 1 << uint32_t(dof)) || !v ? 0 :
					m_dof_indexes[id] < nu ? v[m_dof_indexes[id]] : 
					m_mesh->model()->boundary()->support(m_dof_indexes[id] - nu)->velocity(t);
			}
			double Node::acceleration(DOF dof) const
			{
				//data
				const uint8_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const double t = m_mesh->model()->analysis()->solver()->time_new();
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const double* a = m_mesh->model()->analysis()->solver()->acceleration_new();
				//return
				return 
					(~m_dof_set & 1 << uint32_t(dof)) || !a ? 0 :
					m_dof_indexes[id] < nu ? a[m_dof_indexes[id]] : 
					m_mesh->model()->boundary()->support(m_dof_indexes[id] - nu)->acceleration(t);
			}

			double Node::state(DOF dof, uint32_t step) const
			{
				//data
				const uint8_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const double* x = dynamic_cast<math::solvers::Incremental*>(m_mesh->model()->analysis()->solver())->state_data();
				//return
				return (m_dof_set & 1 << uint32_t(dof)) && m_dof_indexes[id] < nu ? x[m_dof_indexes[id] + nu * step] : 0;
			}
			double Node::velocity(DOF dof, uint32_t step) const
			{
				//data
				const uint8_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const double* v = dynamic_cast<math::solvers::Incremental*>(m_mesh->model()->analysis()->solver())->velocity_data();
				//return
				return (m_dof_set & 1 << uint32_t(dof)) && m_dof_indexes[id] < nu ? v[m_dof_indexes[id] + nu * step] : 0;
			}
			double Node::acceleration(DOF dof, uint32_t step) const
			{
				//data
				const uint8_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const double* a = dynamic_cast<math::solvers::Incremental*>(m_mesh->model()->analysis()->solver())->acceleration_data();
				//return
				return (m_dof_set & 1 << uint32_t(dof)) && m_dof_indexes[id] < nu ? a[m_dof_indexes[id] + nu * step] : 0;
			}

			const double* Node::position(void) const
			{
				return m_position;
			}
			const double* Node::position(const double* position)
			{
				return (double*) memcpy(m_position, position, sizeof(m_position));
			}
			const double* Node::position(double x1, double x2, double x3)
			{
				m_position[0] = x1;
				m_position[1] = x2;
				m_position[2] = x3;
				return m_position;
			}

			math::GSO3 Node::rotation_old(void) const
			{
				return m_rotation ? *m_rotation : math::GSO3();
			}
			math::GSO3 Node::rotation_gso3(void) const
			{
				return m_rotation ? rotation_aso3().exp() * *m_rotation : math::GSO3();
			}
			math::ASO3 Node::rotation_aso3(void) const
			{
				//data
				double* x = m_mesh->model()->analysis()->solver()->state_new();
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const uint32_t id = math::bit_index(m_dof_set, 1 << uint32_t(DOF::Rotation_1));
				//return
				return math::ASO3(
					m_rotation && m_dof_indexes[id + 0] < nu ? x[m_dof_indexes[id + 0]] : 0,
					m_rotation && m_dof_indexes[id + 1] < nu ? x[m_dof_indexes[id + 1]] : 0,
					m_rotation && m_dof_indexes[id + 2] < nu ? x[m_dof_indexes[id + 2]] : 0
				);
			}

			//analysis
			void Node::setup(void)
			{
				//data
				const uint32_t dof_rotation = 
					1 << uint32_t(DOF::Rotation_1) |
					1 << uint32_t(DOF::Rotation_2) |
					1 << uint32_t(DOF::Rotation_3);
				//rotation
				if((m_dof_set & dof_rotation) == dof_rotation)
				{
					delete m_rotation;
					m_rotation = new math::GSO3;
				}
			}
			void Node::update(void)
			{
				if(m_rotation) *m_rotation = rotation_gso3();
			}
			void Node::restore(void)
			{
				//data
				double* x = m_mesh->model()->analysis()->solver()->state_new();
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const uint32_t id = math::bit_index(m_dof_set, 1 << uint32_t(DOF::Rotation_1));
				//restore
				if(m_rotation)
				{
					if(m_dof_indexes[id + 0] < nu) x[m_dof_indexes[id + 0]] = 0;
					if(m_dof_indexes[id + 1] < nu) x[m_dof_indexes[id + 1]] = 0;
					if(m_dof_indexes[id + 2] < nu) x[m_dof_indexes[id + 2]] = 0;
				}
			}
			void Node::dof_setup(uint32_t& dof_counter)
			{
				//data
				const uint8_t ndof = math::bit_count(m_dof_set);
				//dofs
				m_dof_indexes.resize(ndof);
				for(uint32_t i = 0; i < ndof; i++)
				{
					m_dof_indexes[i] = dof_counter++;
				}
			}

			//static
			Mesh* Node::m_mesh = nullptr;
		}
	}
}
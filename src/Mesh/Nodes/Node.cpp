//std
#include <cstring>
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Assembler.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

//math
#include "Math/inc/Linear/Vec3.hpp"
#include "Math/inc/Linear/Quat.hpp"
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Solvers/Incremental.hpp"

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			//constructor
			Node::Node(const double* x) : 
				m_dof_set{0}, m_position_ref{x[0], x[1], x[2]}, m_position_new{0, 0, 0}, m_quaternion_old{nullptr}, m_quaternion_new{nullptr}
			{
				return;
			}
			Node::Node(double x1, double x2, double x3) : 
				m_dof_set{0}, m_position_ref{x1, x2, x3}, m_position_new{0, 0, 0}, m_quaternion_old{nullptr}, m_quaternion_new{nullptr}
			{
				return;
			}

			//destructor
			Node::~Node(void)
			{
				return;
			}

			//serialization
			void Node::save(FILE* file) const
			{
				fprintf(file, "Index: %d Position: %+.6e %+.6e %+.6e", m_index, m_position_ref[0], m_position_ref[1], m_position_ref[2]);
			}

			//data
			uint32_t Node::index(void) const
			{
				return m_index;
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
				const uint32_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const double t = m_mesh->model()->analysis()->solver()->time_new();
				const double* x = m_mesh->model()->analysis()->solver()->state_new();
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				//return
				return ~m_dof_set & 1 << uint32_t(dof) ? 0 :
					m_dof_indexes[id] < nu ? x[m_dof_indexes[id]] : 
					m_mesh->model()->boundary()->support(m_dof_indexes[id] - nu)->state(t);
			}
			double Node::velocity(DOF dof) const
			{
				//data
				const uint32_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const double t = m_mesh->model()->analysis()->solver()->time_new();
				const double* v = m_mesh->model()->analysis()->solver()->velocity_new();
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				//return
				return ~m_dof_set & 1 << uint32_t(dof) ? 0 :
					m_dof_indexes[id] < nu ? v[m_dof_indexes[id]] : 
					m_mesh->model()->boundary()->support(m_dof_indexes[id] - nu)->velocity(t);
			}
			double Node::acceleration(DOF dof) const
			{
				//data
				const uint32_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const double t = m_mesh->model()->analysis()->solver()->time_new();
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const double* a = m_mesh->model()->analysis()->solver()->acceleration_new();
				//return
				return ~m_dof_set & 1 << uint32_t(dof) ? 0 :
					m_dof_indexes[id] < nu ? a[m_dof_indexes[id]] : 
					m_mesh->model()->boundary()->support(m_dof_indexes[id] - nu)->acceleration(t);
			}

			double Node::state(DOF dof, uint32_t step) const
			{
				//data
				const uint32_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const double* x = dynamic_cast<math::solvers::Incremental*>(m_mesh->model()->analysis()->solver())->state_data();
				//return
				return (m_dof_set & 1 << uint32_t(dof)) && m_dof_indexes[id] < nu ? x[m_dof_indexes[id] + nu * step] : 0;
			}
			double Node::velocity(DOF dof, uint32_t step) const
			{
				//data
				const uint32_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const double* v = dynamic_cast<math::solvers::Incremental*>(m_mesh->model()->analysis()->solver())->velocity_data();
				//return
				return (m_dof_set & 1 << uint32_t(dof)) && m_dof_indexes[id] < nu ? v[m_dof_indexes[id] + nu * step] : 0;
			}
			double Node::acceleration(DOF dof, uint32_t step) const
			{
				//data
				const uint32_t id = math::bit_index(m_dof_set, 1 << uint32_t(dof));
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const double* a = dynamic_cast<math::solvers::Incremental*>(m_mesh->model()->analysis()->solver())->acceleration_data();
				//return
				return (m_dof_set & 1 << uint32_t(dof)) && m_dof_indexes[id] < nu ? a[m_dof_indexes[id] + nu * step] : 0;
			}

			const double* Node::quaternion_old(void) const
			{
				return m_quaternion_old;
			}
			const double* Node::quaternion_new(void) const
			{
				return m_quaternion_new;
			}

			const double* Node::position_ref(void) const
			{
				return m_position_ref;
			}
			const double* Node::position_new(void) const
			{
				return m_position_new;
			}
			const double* Node::position(uint32_t step)
			{
				//data
				const uint32_t ib = 1 << uint32_t(DOF::Translation_1);
				const uint32_t nu = m_mesh->model()->analysis()->assembler()->dof_unknow();
				const double* xd = dynamic_cast<math::solvers::Incremental*>(m_mesh->model()->analysis()->solver())->state_data();
				//position
				for(uint32_t i = 0; i < 3; i++)
				{
					m_position_new[i] = m_position_ref[i];
					const uint32_t id = math::bit_index(m_dof_set, ib << i);
					if((m_dof_set & ib << i) && m_dof_indexes[id] < nu) m_position_new[i] += xd[m_dof_indexes[id] + step * nu];
				}
				//return
				return m_position_new;
			}
			const double* Node::position_ref(double* position_ref)
			{
				return (double*) memcpy(m_position_ref, position_ref, sizeof(m_position_ref));
			}
			const double* Node::position_ref(double x1, double x2, double x3)
			{
				m_position_ref[0] = x1;
				m_position_ref[1] = x2;
				m_position_ref[2] = x3;
				return m_position_ref;
			}

			//analysis
			void Node::setup(void)
			{
				return;
			}
			void Node::compute(void)
			{
				//data
				const double t[] = {
					state(DOF::Rotation_1), state(DOF::Rotation_2), state(DOF::Rotation_3)
				};
				const double u[] = {
					state(DOF::Translation_1), state(DOF::Translation_2), state(DOF::Translation_3)
				};
				//position
				m_position_new[0] = m_position_ref[0] + u[0];
				m_position_new[1] = m_position_ref[1] + u[1];
				m_position_new[2] = m_position_ref[2] + u[2];
				//rotation
				if(m_quaternion_new)
				{
					math::Quat(m_quaternion_new + 0) = math::Vec3(t).quaternion() * math::Quat(m_quaternion_old);
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
					m_dof_indexes[i] = dof_counter + i;
				}
				dof_counter += ndof;
			}

			//static
			Mesh* Node::m_mesh = nullptr;
		}
	}
}
//std
#include <cstring>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vector.hpp"

//FEA
#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Model.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Constraint::Constraint(std::vector<uint32_t> nodes, std::vector<mesh::nodes::dof> dof) : 
			m_nodes{nodes}, m_dof{dof}, m_state_data(nullptr), 
			m_dof_state_data(nullptr), m_dof_velocity_data(nullptr), m_dof_acceleration_data(nullptr), 
			m_function_data(0), m_gradient_data(nullptr), m_hessian_data(nullptr), m_third_data(nullptr)
		{
			return;
		}

		//destructor
		Constraint::~Constraint(void)
		{
			cleanup();
		}

		//serialization
		void Constraint::load(FILE* file)
		{
			//size
			uint32_t nn, dof;
			if(fscanf(file, "%d", &nn) != 1)
			{
				throw std::runtime_error("Constraint loading error!");
			}
			//load
			m_dof.resize(nn);
			m_nodes.resize(nn);
			for(uint32_t i = 0; i < nn; i++)
			{
				if(fscanf(file, "%d %d ", &m_nodes[i], &dof) != 2)
				{
					throw std::runtime_error("Constraint nodes/dof loading error!");
				}
				m_dof[i] = mesh::nodes::dof(dof);
			}
		}
		void Constraint::save(FILE* file) const
		{
			if(fprintf(file, "%d ", (uint32_t) m_nodes.size()) < 0)
			{
				throw std::runtime_error("Constraint saving error!");
			}
			for(uint32_t i = 0; i < m_nodes.size(); i++)
			{
				if(fprintf(file, "%d %d ", m_nodes[i], (uint32_t) m_dof[i]) < 0)
				{
					throw std::runtime_error("Constraint nodes/dof saving error!");
				}
			}
		}

		//data
		Constraint::fun2 Constraint::third(void) const
		{
			return m_third;
		}
		Constraint::fun2 Constraint::third(Constraint::fun2 third)
		{
			return m_third = third;
		}

		Constraint::fun2 Constraint::hessian(void) const
		{
			return m_hessian;
		}
		Constraint::fun2 Constraint::hessian(Constraint::fun2 hessian)
		{
			return m_hessian = hessian;
		}

		Constraint::fun2 Constraint::gradient(void) const
		{
			return m_gradient;
		}
		Constraint::fun2 Constraint::gradient(Constraint::fun2 gradient)
		{
			return m_gradient = gradient;
		}

		Constraint::fun1 Constraint::function(void) const
		{
			return m_function;
		}
		Constraint::fun1 Constraint::function(Constraint::fun1 function)
		{
			return m_function = function;
		}

		std::vector<uint32_t>& Constraint::nodes(void)
		{
			return m_nodes;
		}
		mesh::nodes::Node* Constraint::node(uint32_t index) const
		{
			return m_boundary->model()->mesh()->node(m_nodes[index]);
		}
		const std::vector<uint32_t>& Constraint::nodes(void) const
		{
			return m_nodes;
		}

		std::vector<mesh::nodes::dof>& Constraint::dof(void)
		{
			return m_dof;
		}
		mesh::nodes::dof Constraint::dof(uint32_t index) const
		{
			return m_dof[index];
		}
		const std::vector<mesh::nodes::dof>& Constraint::dof(void) const
		{
			return m_dof;
		}

		//analysis
		void Constraint::compute(void)
		{
			//data
			double* dx = m_dof_state_data;
			double* dv = m_dof_velocity_data;
			double* da = m_dof_acceleration_data;
			const uint32_t ss = m_boundary->model()->analysis()->solver()->state_set();
			//gather
			for(uint32_t i = 0; i < m_dof.size(); i++)
			{
				mesh::nodes::Node* node = m_boundary->model()->mesh()->node(m_nodes[i]);
				if(ss & uint32_t(analysis::solvers::state::x)) dx[i] = node->state(m_dof[i]);
				if(ss & uint32_t(analysis::solvers::state::v)) dv[i] = node->velocity(m_dof[i]);
				if(ss & uint32_t(analysis::solvers::state::a)) da[i] = node->acceleration(m_dof[i]);
			}
			//apply
			m_third(m_third_data, m_dof_state_data);
			m_hessian(m_hessian_data, m_dof_state_data);
			m_function(m_function_data, m_dof_state_data);
			m_gradient(m_gradient_data, m_dof_state_data);
		}
		void Constraint::setup(void)
		{
			m_state_old = m_state_new = 0;
			m_dof_indexes.resize(m_nodes.size());
			for(uint32_t i = 0; i < m_nodes.size(); i++)
			{
				const uint32_t node = m_nodes[i];
				const mesh::nodes::dof dof = m_dof[i];
				m_dof_indexes[i] = m_boundary->model()->mesh()->node(node)->dof_index(dof);
			}
			cleanup();
			allocate();
		}
		void Constraint::cleanup(void)
		{
			delete[] m_state_data;
			delete[] m_third_data;
			delete[] m_hessian_data;
			delete[] m_gradient_data;
			delete[] m_dof_state_data;
			delete[] m_dof_velocity_data;
			delete[] m_dof_acceleration_data;
			m_state_data = m_third_data = m_hessian_data = m_gradient_data = nullptr;
			m_dof_state_data = m_dof_velocity_data = m_dof_acceleration_data = nullptr;
		}
		void Constraint::allocate(void)
		{
			//data
			const uint32_t nd = m_dof.size();
			const uint32_t ns = m_boundary->model()->analysis()->solver()->step_max();
			//allocate dof data
			m_dof_state_data = new double[nd];
			m_dof_velocity_data = new double[nd];
			m_dof_acceleration_data = new double[nd];
			//allocate state data
			m_state_data = new double[ns + 1];
			//allocate functions data
			m_gradient_data = new double[nd];
			m_hessian_data = new double[nd * nd];
			m_third_data = new double[nd * nd * nd];
		}
		bool Constraint::check(void) const
		{
			//data
			char msg[200];
			const char* f1 = "Constraint %d has incompatible lists!";
			const char* f2 = "Constraint %d has out of range nodes!";
			const char* f3 = "Constraint %d has undefined fucntions!";
			const uint32_t nn = m_boundary->model()->mesh()->nodes().size();
			//check
			if(m_nodes.size() != m_dof.size())
			{
				sprintf(msg, f1, m_index);
				throw std::runtime_error(msg);
			}
			for(uint32_t node_index : m_nodes)
			{
				if(node_index >= nn)
				{
					sprintf(msg, f2, m_index);
					throw std::runtime_error(msg);
				}
			}
			if(!m_function || !m_gradient || !m_hessian || !m_third)
			{
				sprintf(msg, f3, m_index);
				throw std::runtime_error(msg);
			}
			//return
			return true;
		}
		void Constraint::state(double* x) const
		{
			for(uint32_t i = 0; i < m_nodes.size(); i++)
			{
				x[i] = m_boundary->model()->mesh()->node(m_nodes[i])->state(m_dof[i]);
			}
		}
		void Constraint::setup_dof(uint32_t& dof_counter)
		{
			m_dof_index = dof_counter++;
		}
		void Constraint::apply_dof(void) const
		{
			for(uint32_t i = 0; i < m_nodes.size(); i++)
			{
				m_boundary->model()->mesh()->node(m_nodes[i])->apply_dof(m_dof[i]);
			}
		}

		void Constraint::record(void)
		{
			m_state_data[m_boundary->model()->analysis()->solver()->step()] = m_state_new;
		}
		void Constraint::update(void)
		{
			m_state_old = m_state_new;
		}
		void Constraint::restore(void)
		{
			m_state_new = m_state_old;
		}
		void Constraint::apply_state(void)
		{
			m_state_new = m_boundary->model()->analysis()->solver()->m_x_new[m_dof_index];
		}
		void Constraint::apply_velocity(void)
		{
			m_state_new = m_boundary->model()->analysis()->solver()->m_v_new[m_dof_index];
		}
		void Constraint::increment_state(void)
		{
			m_state_new = m_state_old + m_boundary->model()->analysis()->solver()->m_dx[m_dof_index];
		}
		void Constraint::apply_acceleration(void)
		{
			m_state_new = m_boundary->model()->analysis()->solver()->m_a_new[m_dof_index];
		}

		std::vector<uint32_t> Constraint::dof_list(void) const
		{
			std::vector<uint32_t> dof_list = m_dof_indexes;
			dof_list.push_back(m_dof_index);
			return dof_list;
		}

		//static data
		Baumgarte Constraint::m_baumgarte;
		Boundary* Constraint::m_boundary = nullptr;
	}
}

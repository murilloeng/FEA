#pragma once

//std
#include <functional>

//FEA
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Boundary/Constraints/Baumgarte.hpp"

namespace fea
{
	namespace mesh
	{
		namespace joints
		{
			class Joint;
		}
	}
	namespace boundary
	{
		class Boundary;
	}
	namespace analysis
	{
		class Assembler;
	}
}

namespace fea
{
	namespace boundary
	{
		class Constraint
		{
		private:
			//constructor
			Constraint(std::vector<uint32_t>, std::vector<mesh::nodes::dof>);

			//destructor
			~Constraint(void);

			//serialization
			void load(FILE*);
			void save(FILE*) const;

			//types
			typedef std::function<void(double&, const double*)> fun1;
			typedef std::function<void(double*, const double*)> fun2;

		public:
			//data
			fun2 third(fun2);
			fun2 third(void) const;

			fun2 hessian(fun2);
			fun2 hessian(void) const;

			fun2 gradient(fun2);
			fun2 gradient(void) const;

			fun1 function(fun1);
			fun1 function(void) const;

			std::vector<uint32_t>& nodes(void);
			mesh::nodes::Node* node(uint32_t) const;
			const std::vector<uint32_t>& nodes(void) const;

			mesh::nodes::dof dof(uint32_t) const;
			std::vector<mesh::nodes::dof>& dof(void);
			const std::vector<mesh::nodes::dof>& dof(void) const;

		private:
			//analysis
			void setup(void);
			void compute(void);
			void cleanup(void);
			void allocate(void);
			bool check(void) const;
			void state(double*) const;
			void setup_dof(uint32_t&);
			void apply_dof(void) const;

			void record(void);
			void update(void);
			void restore(void);
			void apply_state(void);
			void apply_velocity(void);
			void increment_state(void);
			void apply_acceleration(void);

			std::vector<uint32_t> dof_list(void) const;

			//data
			fun2 m_third;
			fun2 m_hessian;
			fun2 m_gradient;
			fun1 m_function;
			uint32_t m_index;
			uint32_t m_dof_index;
			static Boundary* m_boundary;
			static Baumgarte m_baumgarte;
			std::vector<uint32_t> m_nodes;
			std::vector<uint32_t> m_dof_indexes;
			std::vector<mesh::nodes::dof> m_dof;
			double m_state_old, m_state_new, *m_state_data;
			double *m_dof_state_data, *m_dof_velocity_data, *m_dof_acceleration_data;
			double m_function_data, *m_gradient_data, *m_hessian_data, *m_third_data;

			//friends
			friend class Boundary;
			friend class mesh::joints::Joint;
			friend class analysis::Assembler;
		};
	}
}
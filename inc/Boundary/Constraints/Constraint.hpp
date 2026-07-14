#pragma once

//std
#include <vector>
#include <cstdint>
#include <functional>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			enum class DOF : uint32_t;
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
			Constraint(void);

			//destructor
			~Constraint(void);

			//serialization
			void save(FILE*) const;

			//analysis
			void check(void);
			void setup(void);
			void dof_setup(uint32_t&);

			//data
			uint32_t m_dof_index;
			static Boundary* m_boundary;
			std::vector<uint32_t> m_nodes;
			std::vector<uint32_t> m_dof_indexes;
			std::vector<mesh::nodes::DOF> m_dof;
			std::function<void(double*, const double*)> m_hessian;
			std::function<void(double*, const double*)> m_function;
			std::function<void(double&, const double*)> m_gradient;

			//friends
			friend class fea::boundary::Boundary;
			friend class fea::analysis::Assembler;
		};
	}
}
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
}

namespace fea
{
	namespace boundary
	{
		class Constraint
		{
		public:
			//constructor
			Constraint(void);

			//destructor
			~Constraint(void);

			//data
			static Boundary* m_boundary;
			std::vector<uint32_t> m_nodes;
			std::vector<mesh::nodes::DOF> m_dof;
			std::function<void(double*, const double*)> m_hessian;
			std::function<void(double*, const double*)> m_function;
			std::function<void(double&, const double*)> m_gradient;
		};
	}
}
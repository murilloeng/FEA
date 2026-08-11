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

			//types
			typedef std::function<void(double*, const double*)> Hessian;
			typedef std::function<void(double*, const double*)> Gradient;
			typedef std::function<void(double&, const double*)> Function;

		public:
			//data
			Hessian hessian(Hessian);
			Hessian hessian(void) const;

			Gradient gradient(Gradient);
			Gradient gradient(void) const;

			Function function(Function);
			Function function(void) const;

			const std::vector<uint32_t>& nodes(void) const;
			const std::vector<mesh::nodes::DOF>& dof(void) const;

			//index
			uint32_t index(void) const;

		private:
			//analysis
			void check(void);
			void setup(void);
			void state(double*) const;
			void dof_setup(uint32_t&);

			//data
			uint32_t m_index;
			Hessian m_hessian;
			Gradient m_gradient;
			Function m_function;
			uint32_t m_dof_index;
			static Boundary* m_boundary;
			std::vector<uint32_t> m_nodes;
			std::vector<uint32_t> m_dof_indexes;
			std::vector<mesh::nodes::DOF> m_dof;

			//friends
			friend class fea::boundary::Boundary;
			friend class fea::analysis::Assembler;
		};
	}
}
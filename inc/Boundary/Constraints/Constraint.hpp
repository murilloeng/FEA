#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>
#include <functional>

namespace fea
{
	namespace mesh
	{
		class Mesh;
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
			typedef std::function<void(double*, const double*)> Gradient;
			typedef std::function<void(double&, const double*)> Function;

		public:
			//data
			static double beta(void);
			static double beta(double);

			static double alpha(void);
			static double alpha(double);

			Gradient third(Gradient);
			Gradient third(void) const;

			Gradient hessian(Gradient);
			Gradient hessian(void) const;

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
			void dof_setup(uint32_t&);
			void state(double*) const;
			void velocity(double*) const;
			void acceleration(double*) const;

			//data
			uint32_t m_index;
			Gradient m_third;
			Gradient m_hessian;
			Gradient m_gradient;
			Function m_function;
			uint32_t m_dof_index;
			static double m_beta;
			static double m_alpha;
			static Boundary* m_boundary;
			std::vector<uint32_t> m_nodes;
			std::vector<uint32_t> m_dof_indexes;
			std::vector<mesh::nodes::DOF> m_dof;

			//friends
			friend class fea::mesh::Mesh;
			friend class fea::boundary::Boundary;
			friend class fea::analysis::Assembler;
		};
	}
}
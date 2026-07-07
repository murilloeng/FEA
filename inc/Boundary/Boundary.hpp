#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	class Model;
	namespace mesh
	{
		namespace nodes
		{
			enum class DOF : uint32_t;
		}
	}
	namespace boundary
	{
		class Initial;
		class Support;
		class LoadCase;
		class Constraint;
		class Dependency;
		class LoadCombination;
	}
}

namespace fea
{
	namespace boundary
	{
		class Boundary
		{
		public:
			//constructor
			Boundary(void);

			//destructor
			~Boundary(void);

			//analysis
			void check(void);
			void setup(void);
			void dof_apply(void);
			void dof_setup(uint32_t&);

			//create
			void create_support(uint32_t, mesh::nodes::DOF);
			
			void create_load_combination(void);
			void create_load_combination(uint32_t, bool, double);

			void create_load_case(void);
			void create_load_case(uint32_t, mesh::nodes::DOF, double);

			//data
			static Model* m_model;
			std::vector<Initial*> m_initials;
			std::vector<Support*> m_supports;
			std::vector<LoadCase*> m_load_cases;
			std::vector<Constraint*> m_constraints;
			std::vector<Dependency*> m_dependencies;
			std::vector<LoadCombination*> m_load_combinations;
		};
	}
}
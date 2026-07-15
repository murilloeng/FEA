#pragma once

//std
#include <vector>
#include <cstdio>
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
	namespace analysis
	{
		class Assembler;
	}
}

namespace fea
{
	namespace boundary
	{
		class Boundary
		{
		private:
			//constructor
			Boundary(void);

			//destructor
			~Boundary(void);

			//serialization
			void save(FILE*) const;

		public:
			//data
			static Model* model(void);

			Initial* initial(uint32_t) const;
			Support* support(uint32_t) const;
			LoadCase* load_case(uint32_t) const;
			Constraint* constraint(uint32_t) const;
			Dependency* dependency(uint32_t) const;
			LoadCombination* load_combination(uint32_t) const;

			const std::vector<Initial*>& initials(void) const;
			const std::vector<Support*>& supports(void) const;
			const std::vector<LoadCase*>& load_cases(void) const;
			const std::vector<Constraint*>& constraints(void) const;
			const std::vector<Dependency*>& dependencies(void) const;
			const std::vector<LoadCombination*>& load_combinations(void) const;

			//create
			void create_support(uint32_t, mesh::nodes::DOF);
			
			void create_load_combination(void);
			void create_load_combination(uint32_t, bool, double);
			
			void create_load_case(void);
			void create_load_case(uint32_t, mesh::nodes::DOF, double);

		private:
			//analysis
			void clear(void);
			void check(void);
			void setup(void);
			void dof_apply(void);
			void dof_setup(uint32_t&);

			//data
			static Model* m_model;
			std::vector<Initial*> m_initials;
			std::vector<Support*> m_supports;
			std::vector<LoadCase*> m_load_cases;
			std::vector<Constraint*> m_constraints;
			std::vector<Dependency*> m_dependencies;
			std::vector<LoadCombination*> m_load_combinations;

			//friends
			friend class fea::Model;
			friend class fea::analysis::Assembler;
		};
	}
}
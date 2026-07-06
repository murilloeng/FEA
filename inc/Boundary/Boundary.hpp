#pragma once

//std
#include <vector>

namespace fea
{
	class Model;
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
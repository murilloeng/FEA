#pragma once

//std
#include <cstdio>
#include <vector>
#include <climits>
#include <cstdint>

namespace fea
{
	namespace models
	{
		class Model;
	}
	namespace mesh
	{
		class Mesh;
		namespace nodes
		{
			enum class dof : uint32_t;
		}
	}
	namespace analysis
	{
		class Analysis;
		class Assembler;
	}
	namespace boundary
	{
		class Initial;
		class Support;
		class Constraint;
		class Dependency;
		namespace time
		{
			class Time;
			enum class Type : uint32_t;
		}
		namespace loads
		{
			class LoadCombination;
			class LoadCase;
			enum class Type : uint32_t;
		}
	}
}

namespace fea
{
	namespace boundary
	{
		class Boundary
		{
		private:
			//constructors
			Boundary(void);

			//destructor
			~Boundary(void);

			//serialization
			void load(FILE*);
			void load_times(FILE*);
			void load_initials(FILE*);
			void load_supports(FILE*);
			void load_load_cases(FILE*);
			void load_constraints(FILE*);
			void load_dependencies(FILE*);
			void load_load_combinations(FILE*);

			void save(FILE*) const;
			void save_times(FILE*) const;
			void save_initials(FILE*) const;
			void save_supports(FILE*) const;
			void save_load_cases(FILE*) const;
			void save_constraints(FILE*) const;
			void save_dependencies(FILE*) const;
			void save_load_combinations(FILE*) const;

			void load_results(void);
			void save_results(void) const;

		public:
			//model
			models::Model* model(void) const;

			//data
			time::Time* time(uint32_t) const;
			Initial* initial(uint32_t) const;
			Support* support(uint32_t) const;
			Constraint* constraint(uint32_t) const;
			Dependency* dependency(uint32_t) const;
			loads::LoadCase* load_case(uint32_t) const;
			loads::LoadCombination* load_combination(uint32_t) const;

			//lists
			const std::vector<time::Time*>& times(void) const;
			const std::vector<Initial*>& initials(void) const;
			const std::vector<Support*>& supports(void) const;
			const std::vector<Constraint*>& constraints(void) const;
			const std::vector<Dependency*>& dependencies(void) const;
			const std::vector<loads::LoadCase*>& load_cases(void) const;
			const std::vector<loads::LoadCombination*>& load_combinations(void) const;

			//create
			time::Time* create_time(time::Type);

			Initial* create_initial(uint32_t, mesh::nodes::dof, double, double);

			Support* create_support(uint32_t, mesh::nodes::dof);
			Support* create_support(uint32_t, mesh::nodes::dof, uint32_t);

			loads::LoadCombination* create_load_combination(const char* = "");

			Constraint* create_constraint(std::vector<uint32_t>, std::vector<mesh::nodes::dof>);
			Dependency* create_dependency(uint32_t, mesh::nodes::dof, uint32_t, mesh::nodes::dof);

			loads::LoadCase* create_load_case(const char* = "");
			loads::LoadCase* create_load_case(uint32_t, loads::Type, double = 1, uint32_t = UINT_MAX);
			loads::LoadCase* create_load_case(uint32_t, mesh::nodes::dof, double = 1, uint32_t = UINT_MAX);

			//remove
			void remove_time(uint32_t);
			void remove_node(uint32_t);
			void remove_element(uint32_t);
			void remove_initial(uint32_t);
			void remove_support(uint32_t);
			void remove_load_case(uint32_t);
			void remove_constraint(uint32_t);
			void remove_dependency(uint32_t);
			void remove_combination(uint32_t);

			void remove_times(const std::vector<uint32_t>&);
			void remove_nodes(const std::vector<uint32_t>&);
			void remove_elements(const std::vector<uint32_t>&);
			void remove_initials(const std::vector<uint32_t>&);
			void remove_supports(const std::vector<uint32_t>&);
			void remove_load_cases(const std::vector<uint32_t>&);
			void remove_constraints(const std::vector<uint32_t>&);
			void remove_dependencies(const std::vector<uint32_t>&);
			void remove_combinations(const std::vector<uint32_t>&);

		private:
			//analysis
			void check(void);
			void setup(void) const;
			void apply_dof(void) const;
			void setup_dof(uint32_t&) const;

			void mesh_union(void);
			void mesh_split(void);

			void record(void) const;
			void update(void) const;
			void restore(void) const;

			//merge
			void merge_nodes(uint32_t, uint32_t) const;
			void merge_elements(uint32_t, uint32_t) const;

		private:
			//data
			static models::Model* m_model;

			std::vector<time::Time*> m_times;
			std::vector<Initial*> m_initials;
			std::vector<Support*> m_supports;
			std::vector<Constraint*> m_constraints;
			std::vector<Dependency*> m_dependencies;
			std::vector<loads::LoadCase*> m_load_cases;
			std::vector<loads::LoadCombination*> m_load_combinations;

			//friends
			friend class mesh::Mesh;
			friend class models::Model;
			friend class analysis::Analysis;
			friend class analysis::Assembler;
		};
	}
}
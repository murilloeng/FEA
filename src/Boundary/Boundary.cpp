//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"

#include "FEA/inc/Boundary/Initials/Initial.hpp"

#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Boundary/Loads/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"

#include "FEA/inc/Boundary/Constraints/Constraint.hpp"

#include "FEA/inc/Boundary/Dependencies/Dependency.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Boundary::Boundary(void)
		{
			Initial::m_boundary = this;
			Support::m_boundary = this;
			LoadCase::m_boundary = this;
			Constraint::m_boundary = this;
			Dependency::m_boundary = this;
			loads::Load::m_boundary = this;
			LoadCombination::m_boundary = this;
		}
	
		//destructor
		Boundary::~Boundary(void)
		{
			for(const Initial* initial : m_initials) delete initial;
			for(const Support* support : m_supports) delete support;
			for(const LoadCase* load_case : m_load_cases) delete load_case;
			for(const Constraint* constraint : m_constraints) delete constraint;
			for(const Dependency* dependency : m_dependencies) delete dependency;
			for(const LoadCombination* load_combination : m_load_combinations) delete load_combination;
		}

		//serialization
		void Boundary::save(FILE* file) const
		{
			//header
			fprintf(file, "# Boundary\n\n");
			//initials
			fprintf(file, "## Initials: %zd\n", m_initials.size());
			for(const Initial* initial : m_initials) initial->save(file), fprintf(file, "\n");
			//Supports
			fprintf(file, "## Supports: %zd\n", m_supports.size());
			//Load cases
			fprintf(file, "## Load cases: %zd\n", m_load_cases.size());
			//Constraints
			fprintf(file, "## Constraints: %zd\n", m_constraints.size());
			//Dependencies
			fprintf(file, "## Dependencies: %zd\n", m_dependencies.size());
			//Load Combinations
			fprintf(file, "## Load combinations: %zd\n", m_load_combinations.size());

		}

		//data
		Model* Boundary::model(void)
		{
			return m_model;
		}

		Initial* Boundary::initial(uint32_t index) const
		{
			return m_initials[index];
		}
		Support* Boundary::support(uint32_t index) const
		{
			return m_supports[index];
		}
		LoadCase* Boundary::load_case(uint32_t index) const
		{
			return m_load_cases[index];
		}
		Constraint* Boundary::constraint(uint32_t index) const
		{
			return m_constraints[index];
		}
		Dependency* Boundary::dependency(uint32_t index) const
		{
			return m_dependencies[index];
		}
		LoadCombination* Boundary::load_combination(uint32_t index) const
		{
			return m_load_combinations[index];
		}

		const std::vector<Initial*>& Boundary::initials(void) const
		{
			return m_initials;
		}
		const std::vector<Support*>& Boundary::supports(void) const
		{
			return m_supports;
		}
		const std::vector<LoadCase*>& Boundary::load_cases(void) const
		{
			return m_load_cases;
		}
		const std::vector<Constraint*>& Boundary::constraints(void) const
		{
			return m_constraints;
		}
		const std::vector<Dependency*>& Boundary::dependencies(void) const
		{
			return m_dependencies;
		}
		const std::vector<LoadCombination*>& Boundary::load_combinations(void) const
		{
			return m_load_combinations;
		}

		//analysis
		void Boundary::check(void)
		{
			for(Initial* initial : m_initials) initial->check();
			for(Support* support : m_supports) support->check();
			for(LoadCase* load_case : m_load_cases) load_case->check();
			for(Constraint* constraint : m_constraints) constraint->check();
			for(Dependency* dependency : m_dependencies) dependency->check();
			for(LoadCombination* load_combination : m_load_combinations) load_combination->check();
		}
		void Boundary::setup(void)
		{
			for(Initial* initial : m_initials) initial->setup();
			for(Support* support : m_supports) support->setup();
			for(LoadCase* load_case : m_load_cases) load_case->setup();
			for(Constraint* constraint : m_constraints) constraint->setup();
			for(Dependency* dependency : m_dependencies) dependency->setup();
			for(LoadCombination* load_combination : m_load_combinations) load_combination->setup();
		}
		void Boundary::dof_apply(void)
		{
			//initials
			for(const Initial* initial : m_initials)
			{
				m_model->mesh()->node(initial->m_node)->m_dof_set |= 1 << uint32_t(initial->m_dof);
			}
			//supports
			for(const Support* support : m_supports)
			{
				m_model->mesh()->node(support->m_node)->m_dof_set |= 1 << uint32_t(support->m_dof);
			}
			//load cases
			for(const LoadCase* load_case : m_load_cases)
			{
				for(const loads::Node* load : load_case->m_loads_nodes)
				{
					m_model->mesh()->node(load->node())->m_dof_set |= 1 << uint32_t(load->dof());
				}
			}
			//constraints
			for(const Constraint* constraint : m_constraints)
			{
				for(uint32_t i = 0; i < constraint->m_nodes.size(); i++)
				{
					m_model->mesh()->node(constraint->m_nodes[i])->m_dof_set |= 1 << uint32_t(constraint->m_dof[i]);
				}
			}
			//dependencies
			for(const Dependency* dependency : m_dependencies)
			{
				m_model->mesh()->node(dependency->m_nodes[0])->m_dof_set |= 1 << uint32_t(dependency->m_dof[0]);
				m_model->mesh()->node(dependency->m_nodes[1])->m_dof_set |= 1 << uint32_t(dependency->m_dof[1]);
			}
		}
		void Boundary::dof_setup(uint32_t& dof_counter)
		{
			for(Constraint* constraint : m_constraints) constraint->dof_setup(dof_counter);
		}

		//create
		void Boundary::create_support(uint32_t node, mesh::nodes::DOF dof)
		{
			m_supports.push_back(new Support(node, dof));
		}

		void Boundary::create_load_combination(void)
		{
			m_load_combinations.push_back(new LoadCombination);
		}
		void Boundary::create_load_combination(uint32_t load_case, bool fixed, double value)
		{
			//data
			LoadCombination* load_combination = new LoadCombination;
			//setup
			load_combination->create_load_item(load_case, value, fixed);
			//append
			m_load_combinations.push_back(load_combination);
		}

		void Boundary::create_load_case(void)
		{
			//data
			LoadCase* load_case = new LoadCase;
			const uint32_t nc = m_load_cases.size();
			//append
			load_case->m_index = nc;
			m_load_cases.push_back(load_case);
		}
		void Boundary::create_load_case(uint32_t node, mesh::nodes::DOF dof, double value)
		{
			//data
			LoadCase* load_case = new LoadCase;
			load_case->create_load_node(node, dof, value);
			//append
			m_load_cases.push_back(load_case);
		}

		//static
		Model* Boundary::m_model = nullptr;
	}
}
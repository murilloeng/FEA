//std
#include <cmath>
#include <cstring>
#include <algorithm>
#include <stdexcept>

//Math
#include "Math/inc/Miscellaneous/bits.hpp"
#include "Math/inc/Miscellaneous/util.hpp"

//FEA
#include "FEA/inc/Model/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Joints/Joint.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Time/Time.hpp"
#include "FEA/inc/Boundary/Time/Types.hpp"
#include "FEA/inc/Boundary/Loads/Item.hpp"
#include "FEA/inc/Boundary/Loads/LoadCase.hpp"
#include "FEA/inc/Boundary/Initials/Initial.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"
#include "FEA/inc/Boundary/Loads/Nodes/Node.hpp"
#include "FEA/inc/Boundary/Loads/LoadCombination.hpp"
#include "FEA/inc/Boundary/Loads/Elements/Element.hpp"
#include "FEA/inc/Boundary/Constraints/Constraint.hpp"
#include "FEA/inc/Boundary/Dependencies/Dependency.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace boundary
	{
		//constructors
		Boundary::Boundary(void)
		{
			Initial::m_boundary = this;
			Support::m_boundary = this;
			Constraint::m_boundary = this;
			Dependency::m_boundary = this;
			time::Time::m_boundary = this;
			loads::LoadCombination::m_boundary = this;
			loads::LoadCase::m_boundary = this;
		}

		//destructor
		Boundary::~Boundary(void)
		{
			//delete
			for(const time::Time* time : m_times) delete time;
			for(const loads::LoadCase* load_case : m_load_cases) delete load_case;
			for(const Initial* initial : m_initials) delete initial;
			for(const Support* support : m_supports) delete support;
			for(const Constraint* constraint : m_constraints) delete constraint;
			for(const Dependency* dependency : m_dependencies) delete dependency;
			for(const loads::LoadCombination* combination : m_load_combinations) delete combination;
		}

		//serialization
		void Boundary::load(FILE* file)
		{
			//header
			math::skip_lines(file, 2);
			//objects
			load_times(file);
			load_initials(file);
			load_supports(file);
			load_load_cases(file);
			load_dependencies(file);
			load_load_combinations(file);
		}
		void Boundary::load_times(FILE* file)
		{
			uint32_t nt, type;
			if(fscanf(file, "## Times: %d", &nt) != 1)
			{
				throw std::runtime_error("Boundary times count loading error!");
			}
			for(uint32_t i = 0; i < nt; i++)
			{
				if(fscanf(file, "%d", &type) != 1)
				{
					throw std::runtime_error("Boundary times type loading error!");
				}
				create_time(time::Type(type))->load(file);
			}
			math::skip_lines(file, 2);
		}
		void Boundary::load_load_cases(FILE* file)
		{
			uint32_t ng;
			if(fscanf(file, "## Load Cases: %d", &ng) != 1)
			{
				throw std::runtime_error("Boundary load cases count loading error!");
			}
			for(uint32_t i = 0; i < ng; i++)
			{
				create_load_case();
				m_load_cases[i]->load(file);
			}
			math::skip_lines(file, 2);
		}
		void Boundary::load_initials(FILE* file)
		{
			uint32_t ni;
			if(fscanf(file, "## Initials: %d", &ni) != 1)
			{
				throw std::runtime_error("Boundary initials count loading error!");
			}
			for(uint32_t i = 0; i < ni; i++)
			{
				create_initial(0, mesh::nodes::dof::translation_1, 0, 0);
				m_initials[i]->load(file);
			}
			math::skip_lines(file, 2);
		}
		void Boundary::load_supports(FILE* file)
		{
			uint32_t ns;
			if(fscanf(file, "## Supports: %d", &ns) != 1)
			{
				throw std::runtime_error("Boundary supports count loading error!");
			}
			for(uint32_t i = 0; i < ns; i++)
			{
				create_support(0, mesh::nodes::dof::translation_1);
				m_supports[i]->load(file);
			}
			math::skip_lines(file, 2);
		}
		void Boundary::load_constraints(FILE* file)
		{
			uint32_t nc;
			if(fscanf(file, "## Constraints: %d", &nc) != 1)
			{
				throw std::runtime_error("Boundary constraints count loading error!");
			}
			for(uint32_t i = 0; i < nc; i++)
			{
				create_constraint({}, {});
				m_constraints[i]->load(file);
			}
			math::skip_lines(file, 2);
		}
		void Boundary::load_dependencies(FILE* file)
		{
			uint32_t nd;
			if(fscanf(file, "## Dependencies: %d", &nd) != 1)
			{
				throw std::runtime_error("Boundary dependencies count loading error!");
			}
			for(uint32_t i = 0; i < nd; i++)
			{
				create_dependency(0, mesh::nodes::dof::translation_1, {}, {});
				m_dependencies[i]->load(file);
			}
			math::skip_lines(file, 2);
		}
		void Boundary::load_load_combinations(FILE* file)
		{
			uint32_t nc;
			if(fscanf(file, "## Load combinations: %d", &nc) != 1)
			{
				throw std::runtime_error("Boundary combinations count loading error!");
			}
			for(uint32_t i = 0; i < nc; i++)
			{
				create_load_combination();
				m_load_combinations[i]->load(file);
			}
			math::skip_lines(file, 2);
		}

		void Boundary::save(FILE* file) const
		{
			//header
			fprintf(file, "# BOUNDARY\n\n");
			//objects
			save_times(file);
			save_initials(file);
			save_supports(file);
			save_load_cases(file);
			save_dependencies(file);
			save_load_combinations(file);
		}
		void Boundary::save_times(FILE* file) const
		{
			fprintf(file, "## Times: %zd\n", m_times.size());
			for(const time::Time* time : m_times)
			{
				fprintf(file, "%02d ", (uint32_t) time->type());
				time->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}
		void Boundary::save_load_cases(FILE* file) const
		{
			fprintf(file, "## Load Cases: %zd\n", m_load_cases.size());
			for(const loads::LoadCase* load_case : m_load_cases)
			{
				load_case->save(file);
			}
			fprintf(file, "\n");
		}
		void Boundary::save_initials(FILE* file) const
		{
			fprintf(file, "## Initials: %zd\n", m_initials.size());
			for(const Initial* initial : m_initials)
			{
				initial->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}
		void Boundary::save_supports(FILE* file) const
		{
			fprintf(file, "## Supports: %zd\n", m_supports.size());
			for(const Support* support : m_supports)
			{
				support->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}
		void Boundary::save_constraints(FILE* file) const
		{
			fprintf(file, "## Constraints: %zd\n", m_constraints.size());
			for(const Constraint* constraint : m_constraints)
			{
				constraint->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}
		void Boundary::save_dependencies(FILE* file) const
		{
			fprintf(file, "## Dependencies: %zd\n", m_dependencies.size());
			for(const Dependency* dependency : m_dependencies)
			{
				dependency->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}
		void Boundary::save_load_combinations(FILE* file) const
		{
			fprintf(file, "## Load combinations: %zd\n", m_load_combinations.size());
			for(const loads::LoadCombination* combination : m_load_combinations)
			{
				combination->save(file);
				fprintf(file, "\n");
			}
			fprintf(file, "\n");
		}

		void Boundary::load_results(void)
		{
			for(Support* support : m_supports)
			{
				support->load_state();
			}
		}
		void Boundary::save_results(void) const
		{
			for(const Support* support : m_supports)
			{
				support->save_state();
			}
		}

		//model
		models::Model* Boundary::model(void) const
		{
			return m_model;
		}

		//data
		time::Time* Boundary::time(uint32_t index) const
		{
			return index < m_times.size() ? m_times[index] : nullptr;
		}
		Initial* Boundary::initial(uint32_t index) const
		{
			return index < m_initials.size() ? m_initials[index] : nullptr;
		}
		Support* Boundary::support(uint32_t index) const
		{
			return index < m_supports.size() ? m_supports[index] : nullptr;
		}
		Constraint* Boundary::constraint(uint32_t index) const
		{
			return index < m_constraints.size() ? m_constraints[index] : nullptr;
		}
		Dependency* Boundary::dependency(uint32_t index) const
		{
			return index < m_dependencies.size() ? m_dependencies[index] : nullptr;
		}
		loads::LoadCombination* Boundary::load_combination(uint32_t index) const
		{
			return index < m_load_combinations.size() ? m_load_combinations[index] : nullptr;
		}
		loads::LoadCase* Boundary::load_case(uint32_t index) const
		{
			return index < m_load_cases.size() ? m_load_cases[index] : nullptr;
		}

		//lists
		const std::vector<time::Time*>& Boundary::times(void) const
		{
			return m_times;
		}
		const std::vector<Initial*>& Boundary::initials(void) const
		{
			return m_initials;
		}
		const std::vector<Support*>& Boundary::supports(void) const
		{
			return m_supports;
		}
		const std::vector<Constraint*>& Boundary::constraints(void) const
		{
			return m_constraints;
		}
		const std::vector<Dependency*>& Boundary::dependencies(void) const
		{
			return m_dependencies;
		}
		const std::vector<loads::LoadCombination*>& Boundary::load_combinations(void) const
		{
			return m_load_combinations;
		}
		const std::vector<loads::LoadCase*>& Boundary::load_cases(void) const
		{
			return m_load_cases;
		}

		//create
		time::Time* Boundary::create_time(time::Type type)
		{
			//data
			time::Time* time = nullptr;
			time::Time::create(time, type);
			//time
			time->m_index = m_times.size();
			//list
			m_times.push_back(time);
			//return
			return time;
		}

		Initial* Boundary::create_initial(uint32_t node, mesh::nodes::dof dof, double u0, double v0)
		{
			//data
			Initial* initial = new Initial;
			//initial
			initial->m_dof = dof;
			initial->m_node = node;
			initial->m_state = u0;
			initial->m_velocity = v0;
			initial->m_index = m_initials.size();
			//list
			m_initials.push_back(initial);
			//return
			return initial;
		}

		Support* Boundary::create_support(uint32_t node, mesh::nodes::dof dof)
		{
			//data
			Support* support = new Support;
			//support
			support->m_dof = dof;
			support->m_node = node;
			support->m_index = m_supports.size();
			//list
			m_supports.push_back(support);
			//return
			return support;
		}
		Support* Boundary::create_support(uint32_t node, mesh::nodes::dof dof, uint32_t state)
		{
			//data
			Support* support = new Support;
			//support
			support->m_dof = dof;
			support->m_node = node;
			support->m_state = state;
			support->m_index = m_supports.size();
			//list
			m_supports.push_back(support);
			//return
			return support;
		}

		loads::LoadCombination* Boundary::create_load_combination(const char* label)
		{
			//data
			loads::LoadCombination* combination = new loads::LoadCombination;
			//combination
			combination->create_item(false, 1, 0);
			combination->m_index = m_load_combinations.size();
			if(label[0] != '\0') strcpy(combination->m_label, label);
			//list
			m_load_combinations.push_back(combination);
			//return
			return combination;
		}

		loads::LoadCase* Boundary::create_load_case(const char* label)
		{
			//data
			loads::LoadCase* load_case = new loads::LoadCase;
			//load case
			load_case->m_index = m_load_cases.size();
			if(label[0] != '\0') strcpy(load_case->m_label, label);
			//list
			m_load_cases.push_back(load_case);
			//return
			return load_case;
		}
		loads::LoadCase* Boundary::create_load_case(uint32_t node, mesh::nodes::dof dof, double value, uint32_t time)
		{
			//data
			loads::LoadCase* load_case = new loads::LoadCase;
			//load case
			load_case->m_index = m_load_cases.size();
			load_case->create_load_node(node, dof, value, time);
			//list
			m_load_cases.push_back(load_case);
			//return
			return load_case;
		}
		loads::LoadCase* Boundary::create_load_case(uint32_t element, loads::Type type, double value, uint32_t time)
		{
			//data
			loads::LoadCase* load_case = new loads::LoadCase;
			//load case
			load_case->m_index = m_load_cases.size();
			load_case->create_load_element(element, type, value, time);
			//list
			m_load_cases.push_back(load_case);
			//return
			return load_case;
		}

		Constraint* Boundary::create_constraint(std::vector<uint32_t> nodes, std::vector<mesh::nodes::dof> dof)
		{
			//data
			Constraint* constraint = new Constraint(nodes, dof);
			//setup
			constraint->m_index = m_constraints.size();
			//list
			m_constraints.push_back(constraint);
			//return
			return constraint;
		}
		Dependency* Boundary::create_dependency(uint32_t n0, mesh::nodes::dof d0, uint32_t n1, mesh::nodes::dof d1)
		{
			//data
			Dependency* dependency = new Dependency(n0, d0, n1, d1);
			//dependency
			dependency->m_index = m_dependencies.size();
			//list
			m_dependencies.push_back(dependency);
			//return
			return dependency;
		}

		//remove
		void Boundary::remove_time(uint32_t index)
		{
			//remove supports
			const uint32_t ns = m_supports.size();
			for(uint32_t i = 0; i < ns; i++)
			{
				const uint32_t j = ns - i - 1;
				const bool ts = m_supports[j]->m_state == index;
				const bool tv = m_supports[j]->m_velocity == index;
				const bool ta = m_supports[j]->m_acceleration == index;
				if(ts || tv || ta) remove_support(j);
				else
				{
					if(m_supports[j]->m_state > index) m_supports[j]->m_state--;
					if(m_supports[j]->m_velocity > index) m_supports[j]->m_velocity--;
					if(m_supports[j]->m_acceleration > index) m_supports[j]->m_acceleration--;
				}
			}
			//remove loads
			for(loads::LoadCase* load_case : m_load_cases)
			{
				const uint32_t nn = load_case->m_loads_nodes.size();
				const uint32_t ne = load_case->m_loads_elements.size();
				for(uint32_t i = 0; i < nn; i++)
				{
					const uint32_t j = nn - i - 1;
					if(load_case->m_loads_nodes[j]->m_time == index)
					{
						load_case->remove_load_node(j);
					}
					else if(load_case->m_loads_nodes[j]->m_time > index)
					{
						load_case->m_loads_nodes[j]->m_time--;
					}
				}
				for(uint32_t i = 0; i < ne; i++)
				{
					const uint32_t j = ne - i - 1;
					if(load_case->m_loads_elements[j]->m_time == index)
					{
						load_case->remove_load_element(j);
					}
					else if(load_case->m_loads_elements[j]->m_time > index)
					{
						load_case->m_loads_elements[j]->m_time--;
					}
				}
			}
			//update indexes
			for(uint32_t i = index + 1; i < m_times.size(); i++)
			{
				m_times[i]->m_index--;
			}
			//remove
			delete m_times[index];
			m_times.erase(m_times.begin() + index);
		}
		void Boundary::remove_node(uint32_t index)
		{
			//remove loads
			for(uint32_t i = 0; i < m_load_cases.size(); i++)
			{
				for(int32_t j = m_load_cases[i]->m_loads_nodes.size() - 1; j >= 0; j--)
				{
					if(m_load_cases[i]->m_loads_nodes[j]->m_node == index)
					{
						m_load_cases[i]->remove_load_node(j);
					}
					else if(m_load_cases[i]->m_loads_nodes[j]->m_node > index)
					{
						m_load_cases[i]->m_loads_nodes[j]->m_node--;
					}
				}
			}
			//remove supports
			for(int32_t i = m_supports.size() - 1; i >= 0; i--)
			{
				if(m_supports[i]->m_node == index)
				{
					remove_support(i);
				}
				else if(m_supports[i]->m_node > index)
				{
					m_supports[i]->m_node--;
				}
			}
			//remove initial
			for(int32_t i = m_initials.size() - 1; i >= 0; i--)
			{
				if(m_initials[i]->m_node == index)
				{
					remove_initial(i);
				}
				else if(m_initials[i]->m_node > index)
				{
					m_initials[i]->m_node--;
				}
			}
			//remove dependency
			for(int32_t i = m_dependencies.size() - 1; i >= 0; i--)
			{
				uint32_t* nodes = m_dependencies[i]->m_nodes;
				if(std::find(nodes, nodes + 2, index) != nodes + 2)
				{
					remove_dependency(i);
				}
				else
				{
					for(uint32_t& node_index : m_dependencies[i]->m_nodes)
					{
						if(node_index > index) node_index--;
					}
				}
			}
		}
		void Boundary::remove_element(uint32_t index)
		{
			for(loads::LoadCase* load_case : m_load_cases)
			{
				for(loads::Element* load : load_case->m_loads_elements)
				{
					if(load->m_element > index) load->m_element--;
					if(load->m_element == index) load_case->remove_load_element(load);
				}
			}
		}
		void Boundary::remove_initial(uint32_t index)
		{
			//indexes
			for(uint32_t i = 0; i < m_initials.size(); i++)
			{
				m_initials[i]->m_index--;
			}
			//remove
			delete m_initials[index];
			m_initials.erase(m_initials.begin() + index);
		}
		void Boundary::remove_support(uint32_t index)
		{
			//indexes
			for(uint32_t i = 0; i < m_supports.size(); i++)
			{
				m_supports[i]->m_index--;
			}
			//remove
			delete m_supports[index];
			m_supports.erase(m_supports.begin() + index);
		}
		void Boundary::remove_combination(uint32_t index)
		{
			//remove
			delete m_load_combinations[index];
			m_load_combinations.erase(m_load_combinations.begin() + index);
			//indexes
			for(uint32_t i = index; i < m_load_combinations.size(); i++)
			{
				m_load_combinations[i]->m_index--;
			}
		}
		void Boundary::remove_load_case(uint32_t index)
		{
			//update combinations
			for(loads::LoadCombination* combination : m_load_combinations)
			{
				for(int32_t j = combination->m_items.size() - 1; j >=0; j--)
				{
					if(combination->m_items[j]->load_case() == index)
					{
						combination->remove_item(j);
					}
					else if(combination->m_items[j]->load_case() > index)
					{
						combination->m_items[j]->load_case(combination->m_items[j]->load_case() - 1);
					}
				}
			}
			//indexes
			for(uint32_t i = index; i < m_load_cases.size(); i++)
			{
				m_load_cases[i]->m_index--;
			}
			//remove load case
			delete m_load_cases[index];
			m_load_cases.erase(m_load_cases.begin() + index);
		}
		void Boundary::remove_constraint(uint32_t index)
		{
			//indexes
			for(uint32_t i = index + 1; i < m_constraints.size(); i++)
			{
				m_constraints[i]->m_index--;
			}
			//remove
			delete m_constraints[index];
			m_constraints.erase(m_constraints.begin() + index);
		}
		void Boundary::remove_dependency(uint32_t index)
		{
			//indexes
			for(uint32_t i = index + 1; i < m_dependencies.size(); i++)
			{
				m_dependencies[i]->m_index--;
			}
			//remove
			delete m_dependencies[index];
			m_dependencies.erase(m_dependencies.begin() + index);
		}

		void Boundary::remove_times(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_time(list[i]);
			}
		}
		void Boundary::remove_nodes(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_node(list[i]);
			}
		}
		void Boundary::remove_load_cases(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_load_case(list[i]);
			}
		}
		void Boundary::remove_elements(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_element(list[i]);
			}
		}
		void Boundary::remove_initials(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_initial(list[i]);
			}
		}
		void Boundary::remove_supports(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_time(list[i]);
			}
		}
		void Boundary::remove_constraints(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_constraint(list[i]);
			}
		}
		void Boundary::remove_dependencies(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_dependency(list[i]);
			}
		}
		void Boundary::remove_combinations(const std::vector<uint32_t>& list)
		{
			for(int32_t i = list.size() - 1; i >= 0; i--)
			{
				remove_combination(list[i]);
			}
		}

		//analysis
		void Boundary::check(void)
		{
			for(const Initial* initial : m_initials) initial->check();
			for(const Support* support : m_supports) support->check();
			for(const Constraint* constraint : m_constraints) constraint->check();
			for(const Dependency* dependency : m_dependencies) dependency->check();
			for(const loads::LoadCase* load_case : m_load_cases) load_case->check();
			for(const loads::LoadCombination* combination : m_load_combinations) combination->check();
		}
		void Boundary::setup(void) const
		{
			for(Initial* initial : m_initials) initial->setup();
			for(Support* support : m_supports) support->setup();
			for(Constraint* constraint : m_constraints) constraint->setup();
			for(Dependency* dependency : m_dependencies) dependency->setup();
			for(loads::LoadCase* load_case : m_load_cases) load_case->setup();
		}
		void Boundary::apply_dof(void) const
		{
			for(const Initial* initial : m_initials) initial->apply_dof();
			for(const Support* support : m_supports) support->apply_dof();
			for(const Constraint* constraint : m_constraints) constraint->apply_dof();
			for(const Dependency* dependency : m_dependencies) dependency->apply_dof();
			for(const loads::LoadCase* load_case : m_load_cases) load_case->apply_dof();
		}
		void Boundary::setup_dof(uint32_t& dof_counter) const
		{
			for(Constraint* constraint : m_constraints) constraint->setup_dof(dof_counter);
		}

		void Boundary::mesh_union(void)
		{
			for(mesh::joints::Joint* joint : m_model->mesh()->m_joints)
			{
				joint->setup_constraints();
				joint->setup_dependencies();
				const std::vector<Constraint*>& constraints = joint->m_constraints;
				const std::vector<Dependency*>& dependencies = joint->m_dependencies;
				m_constraints.insert(m_constraints.end(), constraints.begin(), constraints.end());
				m_dependencies.insert(m_dependencies.end(), dependencies.begin(), dependencies.end());
			}
		}
		void Boundary::mesh_split(void)
		{
			uint32_t njc = 0, njd = 0;
			const uint32_t nc = m_constraints.size();
			const uint32_t nd = m_dependencies.size();
			for(mesh::joints::Joint* joint : m_model->mesh()->m_joints)
			{
				njc += joint->m_constraints.size();
				njd += joint->m_dependencies.size();
			}
			m_constraints.resize(nc - njc);
			m_dependencies.resize(nd - njd);
		}

		void Boundary::record(void) const
		{
			for(Support* support : m_supports)
			{
				support->record();
			}
			for(Constraint* constraint : m_constraints)
			{
				constraint->record();
			}
		}
		void Boundary::update(void) const
		{
			for(Constraint* constraint : m_constraints)
			{
				constraint->update();
			}
		}
		void Boundary::restore(void) const
		{
			for(Constraint* constraint : m_constraints)
			{
				constraint->restore();
			}
		}

		//merge
		void Boundary::merge_nodes(uint32_t a, uint32_t b) const
		{
			//initials
			for(Initial* initial : m_initials)
			{
				if(initial->m_node == a) initial->m_node = b;
			}
			//supports
			for(Support* support : m_supports)
			{
				if(support->m_node == a) support->m_node = b;
			}
			//dependencies
			for(Dependency* dependency : m_dependencies)
			{
				for(uint32_t& node : dependency->m_nodes) if(node == a) node = b;
			}
			//constraints
			for(Constraint* constraint : m_constraints)
			{
				for(uint32_t& node : constraint->m_nodes) if(node == a) node = b;
			}
			//load cases
			for(loads::LoadCase* load_case : m_load_cases)
			{
				for(loads::Node* load : load_case->m_loads_nodes) if(load->m_node == a) load->m_node = b;
			}
		}
		void Boundary::merge_elements(uint32_t a, uint32_t b) const
		{
			for(loads::LoadCase* load_case : m_load_cases)
			{
				for(loads::Element* load : load_case->m_loads_elements)
				{
					if(load->m_element == a) load->m_element = b;
				}
			}
		}

		//static data
		models::Model* Boundary::m_model = nullptr;
	}
}
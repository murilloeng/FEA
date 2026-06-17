//std
#include <cmath>
#include <cstring>
#include <algorithm>
#include <stdexcept>

//Math
#include "Math/inc/Linear/Vector.hpp"
#include "Math/inc/Miscellaneous/bits.hpp"

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/Dof.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Mesh/Elements/Types.hpp"
#include "FEA/inc/Mesh/Elements/States.hpp"
#include "FEA/inc/Mesh/Elements/Element.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss2D.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Truss3D.hpp"

#include "FEA/inc/Results/What.hpp"
#include "FEA/inc/Results/Results.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"
#include "FEA/inc/Analysis/Assembler/Assembler.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			//constructors
			Element::Element(void) : m_state_data{nullptr}, m_energy_data{nullptr}
			{
				return;
			}

			//destructor
			Element::~Element(void)
			{
				delete[] m_state_data;
				delete[] m_energy_data;
			}

			//serialization
			void Element::load(FILE* file)
			{
				uint32_t nn;
				if(fscanf(file, "%d", &nn) != 1)
				{
					throw std::runtime_error("Error: Element loading failed!");
				}
				m_nodes.resize(nn);
				for(uint32_t i = 0; i < nn; i++)
				{
					if(fscanf(file, "%d", &m_nodes[i]) != 1)
					{
						throw std::runtime_error("Error: Element loading failed!");
					}
				}
			}
			void Element::save(FILE* file) const
			{
				fprintf(file, "%d ", (uint32_t) m_nodes.size());
				for(const uint32_t node : m_nodes) fprintf(file, "%4d ", node);
			}

			void Element::load_state(void)
			{
				//data
				char buffer[800];
				const uint32_t nn = m_nodes.size();
				const uint32_t nt = math::bit_count(states_set());
				const uint32_t ns = m_mesh->model()->results()->steps();
				sprintf(buffer, "%s/Elements/E%04d.txt", m_mesh->model()->folder().c_str(), m_index);
				//check
				if(!m_mesh->model()->results()->what()->elements()) return;
				//allocate
				delete[] m_state_data;
				m_state_data = new double[(ns + 1) * nn * nt];
				//open
				FILE* file = fopen(buffer, "r");
				//load
				if(!file)
				{
					sprintf(buffer, "Unable to open element %04d results file!", m_index);
					throw std::runtime_error(buffer);
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					for(uint32_t j = 0; j < nn; j++)
					{
						for(uint32_t k = 0; k < nt; k++)
						{
							if(fscanf(file, "%lf", &m_state_data[nn * nt * i + nt * j + k]) != 1)
							{
								fclose(file);
								sprintf(buffer, "Unable to read from element %04d results file!", m_index);
								throw std::runtime_error(buffer);
							}
						}
					}
				}
				//close
				fclose(file);
			}
			void Element::save_state(void) const
			{
				//data
				char buffer[800];
				const uint32_t nn = m_nodes.size();
				const uint32_t nt = math::bit_count(states_set());
				const uint32_t ns = m_mesh->model()->results()->steps();
				sprintf(buffer, "%s/Elements/E%04d.txt", m_mesh->model()->folder().c_str(), m_index);
				//check
				if(!m_mesh->model()->results()->what()->elements()) return;
				//open
				FILE* file = fopen(buffer, "w");
				//save
				if(!file)
				{
					sprintf(buffer, "Unable to create element %04d results file!", m_index);
					throw std::runtime_error(buffer);
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					for(uint32_t j = 0; j < nn; j++)
					{
						for(uint32_t k = 0; k < nt; k++)
						{
							if(fprintf(file, "%+.6e ", m_state_data[nn * nt * i + nt * j + k]) < 0)
							{
								fclose(file);
								sprintf(buffer, "Unable to write to element %04d results file!", m_index);
								throw std::runtime_error(buffer);
							}
						}
					}
					fprintf(file, "\n");
				}
				//close
				fclose(file);
			}

			void Element::load_energy(void)
			{
				//data
				char buffer[800];
				const uint32_t ns = m_mesh->model()->results()->steps();
				sprintf(buffer, "%s/Energy/E%04d.txt", m_mesh->model()->folder().c_str(), m_index);
				//check
				if(!m_mesh->model()->results()->what()->energies()) return;
				//allocate
				delete[] m_energy_data;
				m_energy_data = new double[2 * (ns + 1)];
				//open
				FILE* file = fopen(buffer, "r");
				//load
				if(!file)
				{
					sprintf(buffer, "Unable to open element %04d energy results file!", m_index);
					throw std::runtime_error(buffer);
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					if(fscanf(file, "%lf %lf", &m_energy_data[2 * i + 0], &m_energy_data[2 * i + 1]) != 2)
					{
						fclose(file);
						sprintf(buffer, "Unable to read from element %04d energy results file!", m_index);
						throw std::runtime_error(buffer);
					}
				}
				//close
				fclose(file);
			}
			void Element::save_energy(void) const
			{
				//data
				char buffer[800];
				const uint32_t ns = m_mesh->model()->analysis()->solver()->step();
				sprintf(buffer, "%s/Energy/E%04d.txt", m_mesh->model()->folder().c_str(), m_index);
				//check
				if(!m_mesh->model()->results()->what()->energies()) return;
				//open
				FILE* file = fopen(buffer, "w");
				//save
				if(!file)
				{
					sprintf(buffer, "Unable to create element %04d energy results file!", m_index);
					throw std::runtime_error(buffer);
				}
				for(uint32_t i = 0; i <= ns; i++)
				{
					if(fprintf(file, "%+.6e %+.6e\n", m_energy_data[2 * i + 0], m_energy_data[2 * i + 1]) < 0)
					{
						fclose(file);
						sprintf(buffer, "Unable to write to element %04d energy results file!", m_index);
						throw std::runtime_error(buffer);
					}
				}
				fclose(file);
			}

			//create
			void Element::create(Element*& element, const Element* base)
			{
				create(element, base->type(), base);
			}
			void Element::create(Element*& element, Type type, const Element* base)
			{
				//data
				delete element;
				const std::function<void(void)> factory[] = {
					[&element, type, base](void){ element = base ? new Truss2D(*(Truss2D*) base) : new Truss2D; },
					[&element, type, base](void){ element = base ? new Truss3D(*(Truss3D*) base) : new Truss3D; }
				};
				//create
				for(uint32_t i = 0; 1U << i < uint32_t(Type::last); i++)
				{
					if(uint32_t(type) == 1U << i)
					{
						factory[i]();
						break;
					}
				}
			}

			//data
			Mesh* Element::mesh(void)
			{
				return m_mesh;
			}

			nodes::Node* Element::node(uint32_t index) const
			{
				return m_mesh->node(m_nodes[index]);
			}
			nodes::Node* Element::node(uint32_t index, uint32_t node)
			{
				return m_mesh->node(m_nodes[index] = node);
			}

			//name
			const char* Element::type_name(void) const
			{
				return type_name(type());
			}
			const char* Element::type_name(Type type)
			{
				const char* names[] = {
					"Truss 2D", "Truss 3D"
				};
				return names[math::bit_index(uint32_t(type))];
			}

			//state
			const char* Element::state_name(elements::State state)
			{
				const char* names[] = {
					"E₁₁", "E₂₂", "E₃₃", "E₁₂", "E₁₃", "E₂₃", 
					"S₁₁", "S₂₂", "S₃₃", "S₁₂", "S₁₃", "S₂₃", "Sᵥₘ",
					"Eᵖ₁₁", "Eᵖ₂₂", "Eᵖ₃₃", "Eᵖ₁₂", "Eᵖ₁₃", "Eᵖ₂₃", 
					"F₁", "F₂", "F₃", "M₁", "M₂", "M₃", 
					"F₁₁", "F₂₂", "F₁₂", "F₁₃", "F₂₃", "M₁₁", "M₂₂", "M₁₂", "Q₁", "Q₂", "Q₃"
				};
				for(uint32_t i = 0; elements::State(1ULL << i) < elements::State::last; i++)
				{
					if(elements::State(1ULL << i) == state)
					{
						return names[i];
					}
				}
				return "error";
			}

			//results
			double Element::energy_data(uint32_t type, uint32_t step) const
			{
				return m_energy_data ? m_energy_data[2 * step + type] : 0;
			}
			double Element::state_data(elements::State state, uint32_t node, uint32_t step) const
			{
				//data
				const uint64_t ss = states_set();
				const uint32_t nn = m_nodes.size();
				const uint8_t ns = math::bit_count(ss);
				const uint8_t sp = math::bit_index(ss, uint64_t(state));
				//return
				return m_state_data && ss & uint64_t(state) ? m_state_data[ns * nn * step + node * ns + sp] : 0;
			}

			//list
			const std::vector<uint32_t>& Element::nodes(void) const
			{
				return m_nodes;
			}

			//index
			uint32_t Element::index(void) const
			{
				return m_index;
			}
			uint32_t Element::index_node(uint32_t index) const
			{
				return m_nodes[index];
			}

			//nodes
			void Element::create_node(uint32_t index)
			{
				m_nodes.push_back(index);
			}
			void Element::remove_node(uint32_t index)
			{
				m_nodes.erase(m_nodes.begin() + index);
			}

			//analysis
			void Element::check(void)
			{
				if(m_nodes.empty())
				{
					throw std::runtime_error("Error: Element with no nodes!");
				}
				else
				{
					const uint32_t nn = m_mesh->nodes().size();
					for(uint32_t i = 0; i < m_nodes.size(); i++)
					{
						if(m_nodes[i] >= nn)
						{
							throw std::runtime_error("Error: Element with out of range nodes!");
						}
					}
				}
			}
			void Element::setup(void)
			{
				//data
				uint8_t p;
				const uint32_t nn = m_nodes.size();
				const uint32_t nt = math::bit_count(states_set());
				const uint32_t ns = m_mesh->model()->analysis()->solver()->step_max();
				//dofs
				m_dof_index.clear();
				for(uint32_t i = 0; i < m_nodes.size(); i++)
				{
					const uint32_t ds = dof_set(i);
					for(uint32_t j = 1; j < (uint32_t) nodes::dof::last; j <<= 1)
					{
						if(ds & j)
						{
							p = math::bit_index(node(i)->m_dof_set, j);
							m_dof_index.push_back(node(i)->m_dof_index[p]);
						}
					}
				}
				//reults
				delete[] m_state_data;
				delete[] m_energy_data;
				m_energy_data = new double[2 * (ns + 1)];
				m_state_data = new double[nn * nt * (ns + 1)];
			}
			void Element::record(void)
			{
				return;
			}
			void Element::update(void)
			{
				return;
			}
			void Element::restore(void)
			{
				return;
			}
			void Element::compute(void)
			{
				return;
			}
			void Element::apply_dof(void) const
			{
				for(uint32_t i = 0; i < m_nodes.size(); i++)
				{
					node(i)->apply_dof(dof_set(i));
				}
			}

			//data
			void Element::state(double* d) const
			{
				//data
				uint32_t p = 0;
				const uint32_t ss = m_mesh->model()->analysis()->solver()->state_set();
				const uint32_t hd = ss & uint32_t(analysis::solvers::state::x);
				//state
				for(uint32_t i = 0; i < m_nodes.size(); i++)
				{
					const uint32_t ds = dof_set(i);
					for(uint32_t dof = 1; dof < uint32_t(nodes::dof::last); dof <<= 1)
					{
						if(ds & dof)
						{
							d[p++] = hd ? node(i)->state(nodes::dof(dof)) : 0;
						}
					}
				}
			}
			void Element::velocity(double* v) const
			{
				//data
				uint32_t p = 0;
				const uint32_t ss = m_mesh->model()->analysis()->solver()->state_set();
				const uint32_t hv = ss & uint32_t(analysis::solvers::state::v);
				//velocity
				for(uint32_t i = 0; i < m_nodes.size(); i++)
				{
					const uint32_t ds = dof_set(i);
					for(uint32_t dof = 1; dof < (uint32_t) nodes::dof::last; dof <<= 1)
					{
						if(ds & dof)
						{
							v[p++] = hv ? node(i)->velocity(nodes::dof(dof)) : 0;
						}
					}
				}
			}
			void Element::acceleration(double* a) const
			{
				//data
				uint32_t p = 0;
				const uint32_t ss = m_mesh->model()->analysis()->solver()->state_set();
				const uint32_t ha = ss & uint32_t(analysis::solvers::state::a);
				//acceleration
				for(uint32_t i = 0; i < m_nodes.size(); i++)
				{
					const uint32_t ds = dof_set(i);
					for(uint32_t dof = 1; dof < (uint32_t) nodes::dof::last; dof <<= 1)
					{
						if(ds & dof)
						{
							a[p++] = ha ? node(i)->acceleration((nodes::dof) dof) : 0;
						}
					}
				}
			}

			//tangents
			void Element::inertia(double* M) const
			{
				const uint32_t nd = m_dof_index.size();
				memset(M, 0, nd * nd * sizeof(double));
			}
			void Element::damping(double* C) const
			{
				const uint32_t nd = m_dof_index.size();
				memset(C, 0, nd * nd * sizeof(double));
			}
			void Element::stiffness(double* K) const
			{
				const uint32_t nd = m_dof_index.size();
				memset(K, 0, nd * nd * sizeof(double));
			}

			//forces
			void Element::inertial_force(double* f) const
			{
				//data
				const uint32_t nd = m_dof_index.size();
				double* a = m_mesh->model()->analysis()->assembler()->m_ae;
				double* M = m_mesh->model()->analysis()->assembler()->m_Me;
				//force
				inertia(M);
				acceleration(a);
				math::Vector(f, nd) = math::Matrix(M, nd, nd) * math::Vector(a, nd);
			}
			void Element::internal_force(double* f) const
			{
				//data
				const uint32_t nd = m_dof_index.size();
				double* d = m_mesh->model()->analysis()->assembler()->m_de;
				double* K = m_mesh->model()->analysis()->assembler()->m_Ke;
				//force
				state(d);
				stiffness(K);
				math::Vector(f, nd) = math::Matrix(K, nd, nd) * math::Vector(d, nd);
			}

			//energy
			double Element::kinetic_energy(void) const
			{
				//data
				const uint32_t nd = m_dof_index.size();
				double* v = m_mesh->model()->analysis()->assembler()->m_ve;
				double* M = m_mesh->model()->analysis()->assembler()->m_Me;
				//return
				inertia(M);
				velocity(v);
				return math::Matrix(M, nd, nd).bilinear(v, v) / 2;
			}
			double Element::internal_energy(void) const
			{
				//data
				const uint32_t nd = m_dof_index.size();
				double* d = m_mesh->model()->analysis()->assembler()->m_de;
				double* K = m_mesh->model()->analysis()->assembler()->m_Ke;
				//return
				state(d);
				stiffness(K);
				return math::Matrix(K, nd, nd).bilinear(d, d) / 2;
			}

			//loads
			void Element::reference_force(double* f, const boundary::loads::Element*) const
			{
				memset(f, 0, m_dof_index.size() * sizeof(double));
			}

			//static data
			Mesh* Element::m_mesh = nullptr;
		}
	}
}
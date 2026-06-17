#pragma once

//std
#include <cstdio>
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node;
			enum class dof : uint32_t;
		}
	}
	namespace analysis
	{
		namespace solvers
		{
			class Solver;
		}
	}
}

namespace fea
{
	namespace analysis
	{
		namespace solvers
		{
			class WatchDof
			{
			private:
				//constructors
				WatchDof(Solver*);

				//destructor
				~WatchDof(void);

				//serialization
				void load(FILE*);
				void save(FILE*) const;

			public:
				//data
				double value(void) const;

				double value_min(double);
				double value_min(void) const;

				double value_max(double);
				double value_max(void) const;

				mesh::nodes::Node* node(uint32_t);
				mesh::nodes::Node* node(void) const;

				mesh::nodes::dof dof(void) const;
				mesh::nodes::dof dof(mesh::nodes::dof);

				//index
				uint32_t index_dof(void) const;
				uint32_t index_node(void) const;

			private:
				//solve
				void setup(void);
				bool checkup(void) const;

				//data
				uint32_t m_node;
				Solver* m_solver;
				double m_value_min;
				double m_value_max;
				uint32_t m_dof_index;
				mesh::nodes::dof m_dof;

				//friends
				friend class Solver;
			};
		}
	}
}
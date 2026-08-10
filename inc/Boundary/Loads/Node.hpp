#pragma once

//std
#include <cstdint>

//FEA
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Nodes/Node.hpp"
#include "FEA/inc/Boundary/Loads/Load.hpp"

namespace fea
{
	namespace boundary
	{
		class LoadCase;
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
		namespace loads
		{
			class Node : public Load
			{
			private:
				//constructor
				Node(uint32_t, mesh::nodes::DOF, double);

				//destructor
				~Node(void);

				//serialization
				void save(FILE*) const override;

			public:
				//data
				
				uint32_t index_node(void) const;
				
				mesh::nodes::Node* node(uint32_t);
				mesh::nodes::Node* node(void) const;

				mesh::nodes::DOF dof(void) const;
				mesh::nodes::DOF dof(mesh::nodes::DOF);

			private:
				//analysis
				void check(void);
				void setup(void);

				//data
				uint32_t m_node;
				uint32_t m_dof_index;
				mesh::nodes::DOF m_dof;

				//friends
				friend class fea::boundary::LoadCase;
				friend class fea::analysis::Assembler;
			};
		}
	}
}
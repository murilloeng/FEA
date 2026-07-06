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
			class Node;
		}
		namespace elements
		{
			class Element;
		}
	}
}

namespace fea
{
	namespace mesh
	{
		class Mesh
		{
		public:
			//constructor
			Mesh(void);
	
			//destructor
			~Mesh(void);

			//analysis
			void apply_dof(void);
			void setup_dof(uint32_t&);
	
			//data
			static Model* m_model;
			std::vector<nodes::Node*> m_nodes;
			std::vector<elements::Element*> m_elements;
		};
	}
}
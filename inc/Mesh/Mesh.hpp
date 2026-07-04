#pragma once

//std
#include <vector>

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
	
			//data
			static Model* m_model;
			std::vector<nodes::Node*> m_nodes;
			std::vector<elements::Element*> m_elements;
		};
	}
}
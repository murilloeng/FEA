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

			//data
			static Model* model(void);

			nodes::Node* node(uint32_t) const;
			const std::vector<nodes::Node*>& nodes(void) const;

			elements::Element* element(uint32_t) const;
			const std::vector<elements::Element*>& elements(void) const;

		private:
			//friends
			friend class fea::Model;

			//data
			static Model* m_model;
			std::vector<nodes::Node*> m_nodes;
			std::vector<elements::Element*> m_elements;
		};
	}
}
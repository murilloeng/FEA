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
			enum class Type : uint32_t;
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
			void check(void);
			void setup(void);
			void compute(void);
			void dof_apply(void);
			void dof_setup(uint32_t&);

			//create
			void create_node(const double*);
			void create_node(double, double, double);

			void create_element(elements::Type, std::vector<uint32_t>);
	
			//data
			static Model* m_model;
			std::vector<nodes::Node*> m_nodes;
			std::vector<elements::Element*> m_elements;
		};
	}
}
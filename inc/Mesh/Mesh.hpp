#pragma once

//std
#include <vector>
#include <cstdio>
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
		namespace joints
		{
			class Joint;
			enum class Type : uint32_t;
		}
		namespace elements
		{
			class Element;
			enum class Type : uint32_t;
		}
	}
	namespace analysis
	{
		class Assembler;
	}
}

namespace fea
{
	namespace mesh
	{
		class Mesh
		{
		private:
			//constructor
			Mesh(void);

			//destructor
			~Mesh(void);

			//serialization
			void save(FILE*) const;

		public:
			//data
			static Model* model(void);

			nodes::Node* node(uint32_t) const;
			joints::Joint* joint(uint32_t) const;
			elements::Element* element(uint32_t) const;

			const std::vector<nodes::Node*>& nodes(void) const;
			const std::vector<joints::Joint*>& joints(void) const;
			const std::vector<elements::Element*>& elements(void) const;

			//create
			void create_node(const double*);
			void create_node(double, double, double);

			void append_joint(joints::Joint*);
			void create_joint(joints::Type, std::vector<uint32_t>);

			void append_element(elements::Element*);
			void create_element(elements::Type, std::vector<uint32_t>);

			private:
			//analysis
			void clear(void);
			void check(void);
			void setup(void);
			void update(void);
			void restore(void);
			void compute(void);
			void dof_apply(void);
			void dof_setup(uint32_t&);

			//data
			static Model* m_model;
			std::vector<nodes::Node*> m_nodes;
			std::vector<joints::Joint*> m_joints;
			std::vector<elements::Element*> m_elements;

			//friends
			friend class fea::Model;
			friend class fea::analysis::Assembler;
		};
	}
}
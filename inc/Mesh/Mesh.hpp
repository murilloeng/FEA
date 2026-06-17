#pragma once

//std
#include <cstdio>
#include <vector>
#include <cstdint>

namespace math
{
	class Vec3;
}

namespace fea
{
	class Model;
	namespace mesh
	{
		namespace nodes
		{
			class Node;
			enum class dof : uint32_t;
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
	namespace boundary
	{
		class Boundary;
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
			//constructors
			Mesh(void);

			//destructor
			~Mesh(void);

			//serialization
			void load(FILE*);
			void load_nodes(FILE*);
			void load_joints(FILE*);
			void load_elements(FILE*);

			void save(FILE*) const;
			void save_nodes(FILE*) const;
			void save_joints(FILE*) const;
			void save_elements(FILE*) const;

			void load_results(void);
			void save_results(void) const;

		public:
			//data
			Model* model(void) const;
			nodes::Node* node(uint32_t) const;
			joints::Joint* joint(uint32_t) const;
			elements::Element* element(uint32_t) const;

			//step
			uint32_t step(void) const;

			//lists
			const std::vector<nodes::Node*>& nodes(void) const;
			const std::vector<joints::Joint*>& joints(void) const;
			const std::vector<elements::Element*>& elements(void) const;

			//create
			nodes::Node* create_node(const double*);
			nodes::Node* create_node(double, double, double);

			joints::Joint* create_joint(joints::Type, std::vector<uint32_t> = {});

			elements::Element* create_element(const elements::Element*);
			elements::Element* create_element(elements::Type, std::vector<uint32_t> = {});

			//remove
			void remove_node(uint32_t);
			void remove_joint(uint32_t);
			void remove_element(uint32_t);

			void remove_nodes(const std::vector<uint32_t>&);
			void remove_joints(const std::vector<uint32_t>&);
			void remove_elements(const std::vector<uint32_t>&);

		private:
			//analysis
			void check(void);
			void setup(void);
			void setup_dof(uint32_t&) const;

			void record(void);
			void update(void);
			void restore(void);
			void apply_dof(void);

			//data
			static Model* m_model;
			std::vector<nodes::Node*> m_nodes;
			std::vector<joints::Joint*> m_joints;
			std::vector<elements::Element*> m_elements;

			//friends
			friend class fea::Model;
			friend class fea::boundary::Boundary;
			friend class fea::analysis::Assembler;
		};
	}
}
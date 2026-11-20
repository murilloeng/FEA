#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>

//Math
#include "Math/inc/Groups/ASO3.hpp"
#include "Math/inc/Groups/GSO3.hpp"

namespace fea
{
	namespace mesh
	{
		class Mesh;
		namespace nodes
		{
			enum class DOF : uint32_t;
		}
	}
	namespace boundary
	{
		class Boundary;
	}
	namespace analysis
	{
		class Analysis;
		class Assembler;
		class Incremental;
	}
}

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			class Node
			{
			private:
				//constructor
				Node(const double*);
				Node(const Node&) = delete;
				Node(double, double, double);

				//destructor
				~Node(void);

				//serialization
				void save(FILE*) const;

			public:
				//data
				uint32_t index(void) const;
				uint32_t dof_set(void) const;
				uint32_t dof_index(DOF) const;

				double state(DOF) const;
				double velocity(DOF) const;
				double acceleration(DOF) const;

				double state(DOF, uint32_t) const;
				double velocity(DOF, uint32_t) const;
				double acceleration(DOF, uint32_t) const;

				const double* position(void) const;
				const double* position(const double*);
				const double* position(double, double, double);

				math::GSO3 rotation_old(void) const;
				math::GSO3 rotation_gso3(void) const;
				math::ASO3 rotation_aso3(void) const;

			private:
				//analysis
				void setup(void);
				void update(void);
				void restore(void);
				void dof_setup(uint32_t&);

				//operators
				Node& operator=(const Node&) = delete;

				//data
				uint32_t m_index;
				uint32_t m_dof_set;
				static Mesh* m_mesh;
				double m_position[3];
				math::GSO3* m_rotation;
				std::vector<uint32_t> m_dof_indexes;

				//friends
				friend class fea::mesh::Mesh;
				friend class fea::boundary::Boundary;
				friend class fea::analysis::Analysis;
				friend class fea::analysis::Assembler;
				friend class fea::analysis::Incremental;
			};
		}
	}
}
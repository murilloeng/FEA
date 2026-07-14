#pragma once

//std
#include <vector>
#include <cstdio>
#include <cstdint>

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
				Node(double, double, double);

				//destructor
				~Node(void);

				//serialization
				void save(FILE*) const;

			public:
				//data
				uint32_t index(void) const;
				uint32_t dof_index(DOF) const;
			
				double state(DOF) const;
				double velocity(DOF) const;
				double acceleration(DOF) const;

				double state(DOF, uint32_t) const;
				double velocity(DOF, uint32_t) const;
				double acceleration(DOF, uint32_t) const;

				const double* quaternion_old(void) const;
				const double* quaternion_new(void) const;

				const double* position(uint32_t);
				const double* position_ref(double*);
				const double* position_ref(void) const;
				const double* position_new(void) const;
				const double* position_ref(double, double, double);

			private:
				//analysis
				void setup(void);
				void compute(void);
				void dof_setup(uint32_t&);

				//data
				uint32_t m_index;
				uint32_t m_dof_set;
				static Mesh* m_mesh;
				double m_position_ref[3];
				double m_position_new[3];
				double* m_quaternion_old;
				double* m_quaternion_new;
				std::vector<uint32_t> m_dof_indexes;

				//friends
				friend class fea::mesh::Mesh;
				friend class fea::boundary::Boundary;
				friend class fea::analysis::Analysis;
				friend class fea::analysis::Assembler;
			};
		}
	}
}
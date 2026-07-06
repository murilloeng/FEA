#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	namespace analysis
	{
		class Analysis;
	}
}

namespace fea
{
	namespace analysis
	{
		class Assembler
		{
		public:
			//constructor
			Assembler(void);

			//destructor
			~Assembler(void);

			//dof
			void dof_map(void);
			void dof_sort(void);
			void dof_apply(void);
			void dof_setup(void);
			void dof_local(void);
			
			void dof_triplet_count(void);
			void dof_triplet_apply(void);

			//analysis
			void setup(void);

			//assemble
			void assemble_inertia(double*) const;
			void assemble_damping(double*) const;
			void assemble_stiffness(double*) const;

			void assemble_external_force(double*) const;
			void assemble_internal_force(double*) const;

			void assemble_vector(double*, double*, const std::vector<uint32_t>&) const;

			void assemble_matrix(double*, double*, const std::vector<uint32_t>&) const;
			void assemble_matrix(double*, double*, const std::vector<uint32_t>&, uint32_t) const;
			void assemble_matrix(double*, double*, uint32_t, const std::vector<uint32_t>&) const;

			//data
			double* m_fe;
			double* m_Ae;

			int32_t* m_rows_map;
			int32_t* m_cols_map;
			int32_t* m_rows_triplet;
			int32_t* m_cols_triplet;

			uint32_t m_dof_know;
			uint32_t m_dof_local;
			uint32_t m_dof_total;
			uint32_t m_dof_unknow;
			uint32_t m_dof_triplet;
			uint32_t m_dof_dependent;

			static Analysis* m_analysis;
		};
	}
}
#pragma once

//std
#include <vector>
#include <cstdint>

namespace fea
{
	class Model;
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
		private:
			//constructor
			Assembler(void);

			//destructor
			~Assembler(void);

		public:
			//data
			uint32_t dof_know(void) const;
			uint32_t dof_total(void) const;
			uint32_t dof_unknow(void) const;

			//assemble
			void assemble_inertia(double*) const;
			void assemble_damping(double*) const;
			void assemble_stiffness(double*) const;

			void assemble_dead_force(double*, bool = true, double = 1) const;
			void assemble_external_force(double*, bool = true, double = 1) const;
			void assemble_internal_force(double*, bool = true, double = 1) const;
			void assemble_reference_force(double*, bool = true, double = 1) const;

			void assemble_vector(double*, double*, const std::vector<uint32_t>&, double = 1) const;

			void assemble_matrix(double*, double*, const std::vector<uint32_t>&, double = 1) const;
			void assemble_matrix(double*, double*, const std::vector<uint32_t>&, uint32_t, double = 1) const;
			void assemble_matrix(double*, double*, uint32_t, const std::vector<uint32_t>&, double = 1) const;

		private:
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

			//friends
			friend class fea::Model;
			friend class fea::analysis::Analysis;
		};
	}
}
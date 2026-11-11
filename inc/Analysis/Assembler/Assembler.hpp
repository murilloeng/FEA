#pragma once

//std
#include <vector>
#include <cstdint>

//FEA
#include "FEA/inc/Analysis/Assembler/Compute.hpp"
#include "FEA/inc/Analysis/Assembler/Request.hpp"

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
			Compute& compute(void);
			Request& request(void);
			uint32_t dof_know(void) const;
			uint32_t dof_total(void) const;
			uint32_t dof_unknow(void) const;

			//apply
			void apply_state(const double*) const;
			void apply_velocity(const double*) const;
			void apply_acceleration(const double*) const;

			//assemble
			void assemble(void);
			void assemble_loads(void);
			void assemble_elements(void);
			void assemble_constraints(void);

			void assemble_vector(double*, const double*, const std::vector<uint32_t>&, double = 1) const;

			void assemble_matrix(double*, const double*, const std::vector<uint32_t>&, double = 1) const;
			void assemble_matrix(double*, const double*, const std::vector<uint32_t>&, uint32_t, double = 1) const;
			void assemble_matrix(double*, const double*, uint32_t, const std::vector<uint32_t>&, double = 1) const;

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
			int32_t* m_rows_map;
			int32_t* m_cols_map;
			int32_t* m_rows_triplet;
			int32_t* m_cols_triplet;

			Compute m_compute;
			Request m_request;
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
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
			void dof_count(void);

			void dof_add(uint32_t, uint32_t);
			void dof_add(const std::vector<uint32_t>&);

			void dof_count(uint32_t);
			void dof_count(const std::vector<uint32_t>&);

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
		};
	}
}
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
			void map_dof(void);
			void sort_dof(void);
			void apply_dof(void);
			void setup_dof(void);
			void count_dof(void);
			void count_dof(uint32_t);
			void count_dof(const std::vector<uint32_t>&);

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
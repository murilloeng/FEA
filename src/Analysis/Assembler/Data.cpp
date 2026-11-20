//std
#include <cstring>

//FEA
#include "FEA/inc/Analysis/Assembler/Data.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Data::Data(void) : 
			m_size{0}, m_request{nullptr}, m_T{0}, m_U{0}, m_V{0},
			m_x{nullptr}, m_v{nullptr}, m_a{nullptr}, m_f{nullptr}, m_K{nullptr}, m_C{nullptr}, m_M{nullptr}, m_A{nullptr}
		{
			return;
		}

		//destructor
		Data::~Data(void)
		{
			cleanup();
		}

		//setup
		void Data::reset(void)
		{
			m_T = m_U = m_V = 0;
			memset(m_f, 0, m_size * sizeof(double));
			memset(m_K, 0, m_size * m_size * sizeof(double));
			memset(m_C, 0, m_size * m_size * sizeof(double));
			memset(m_M, 0, m_size * m_size * sizeof(double));
			memset(m_A, 0, m_size * m_size * m_size * sizeof(double));
		}
		void Data::cleanup(void)
		{
			double** data[] = {
				&m_x, &m_v, &m_a, &m_f, &m_K, &m_C, &m_M, &m_A
			};
			for(double** ptr : data)
			{
				delete[] *ptr;
				*ptr = nullptr;
			}
		}
		void Data::setup(uint32_t size)
		{
			cleanup();
			m_size = size;
			m_x = new double[size];
			m_v = new double[size];
			m_a = new double[size];
			m_f = new double[size];
			m_K = new double[size * size];
			m_C = new double[size * size];
			m_M = new double[size * size];
			m_A = new double[size * size * size];
		}
	}
}
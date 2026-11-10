//FEA
#include "FEA/inc/Analysis/Assembler/Compute.hpp"

namespace fea
{
	namespace analysis
	{
		//constructor
		Compute::Compute(void) : 
			m_T{0}, m_U{0}, m_V{0},
			m_fi{nullptr}, m_fe{nullptr}, 
			m_K{nullptr}, m_C{nullptr}, m_M{nullptr}
		{
			return;
		}

		//destructor
		Compute::~Compute(void)
		{
			cleanup();
		}

		//setup
		void Compute::cleanup(void)
		{
			double** data[] = {
				&m_fi, &m_fe, 
				&m_K, &m_C, &m_M
			};
			for(double** ptr : data)
			{
				delete[] *ptr;
				*ptr = nullptr;
			}
		}
		void Compute::setup(uint32_t nd)
		{
			cleanup();
			m_x = new double[nd];
			m_fi = new double[nd];
			m_fe = new double[nd];
			m_K = new double[nd * nd];
			m_C = new double[nd * nd];
			m_M = new double[nd * nd];
		}
	}
}
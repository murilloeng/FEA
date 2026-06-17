//FEA
#include "FEA/inc/Boundary/Support.hpp"
#include "FEA/inc/Boundary/Boundary.hpp"

namespace fea
{
	namespace boundary
	{
		//constructor
		Boundary::Boundary(void)
		{
			return;
		}

		//destructor
		Boundary::~Boundary(void)
		{
			for(const Support* support : m_supports) delete support;
		}

		//data
		Model* Boundary::model(void)
		{
			return m_model;
		}

		Support* Boundary::support(uint32_t index) const
		{
			return m_supports[index];
		}
		const std::vector<Support*>& Boundary::supports(void) const
		{
			return m_supports;
		}

		//analysis
		void Boundary::check(void)
		{
			for(Support* support : m_supports) support->check();
		}
		void Boundary::setup(void)
		{
			for(Support* support : m_supports) support->setup();
		}
		void Boundary::record(void)
		{
			for(Support* support : m_supports) support->record();
		}
		void Boundary::update(void)
		{
			return;
		}
		void Boundary::restore(void)
		{
			return;
		}
		void Boundary::compute(void)
		{
			return;
		}

		//static data
		Model* Boundary::m_model = nullptr;
	}
}
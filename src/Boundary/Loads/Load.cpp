//FEA
#include "FEA/inc/Boundary/Loads/Load.hpp"

namespace fea
{
	namespace boundary
	{
		namespace loads
		{
			//constructor
			Load::Load(void) : m_value{0}
			{
				return;
			}
		
			//destructor
			Load::~Load(void)
			{
				return;
			}

			//analysis
			void Load::check(void)
			{
				return;
			}
			void Load::setup(void)
			{
				return;
			}

			//static
			Boundary* Load::m_boundary = nullptr;
		}
	}
}
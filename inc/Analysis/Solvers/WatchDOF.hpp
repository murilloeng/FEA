#pragma once

//std
#include <cstdint>

namespace fea
{
	namespace mesh
	{
		namespace nodes
		{
			enum class DOF : uint32_t;
		}
	}
	namespace analysis
	{
		class Analysis;
	}
}

namespace fea
{
	namespace analysis
	{
		class WatchDOF
		{
		public:
			//constructor
			WatchDOF(void);

			//destructor
			~WatchDOF(void);

			//analysis
			void check(void);

			//data
			uint32_t m_node;
			mesh::nodes::DOF m_dof;
			static Analysis* m_analysis;
		};
	}
}
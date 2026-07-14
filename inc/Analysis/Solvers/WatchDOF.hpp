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
		class Solver;
		class Analysis;
	}
}

namespace fea
{
	namespace analysis
	{
		class WatchDOF
		{
		private:
			//constructor
			WatchDOF(void);

			//destructor
			~WatchDOF(void);

		public:
			//data
			uint32_t node(uint32_t);
			uint32_t node(void) const;

			mesh::nodes::DOF dof(void) const;
			mesh::nodes::DOF dof(mesh::nodes::DOF);

			//state
			double state(void) const;
			double velocity(void) const;
			double acceleration(void) const;

		private:
			//analysis
			void check(void);

			//data
			uint32_t m_node;
			mesh::nodes::DOF m_dof;
			static Analysis* m_analysis;

			//friends
			friend class fea::analysis::Solver;
			friend class fea::analysis::Analysis;
		};
	}
}
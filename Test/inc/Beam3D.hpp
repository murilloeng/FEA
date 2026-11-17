#pragma once

namespace test
{
	namespace beam3D
	{
		void test(void);
		namespace elastic
		{
			void lee_frame(void);
			void cantilever_force(void);
			void cantilever_bending(void);
		}
		namespace inelastic
		{
			void pinned_force(void);
			void pinned_bending(void);
			void cantilever_force(void);
			void cantilever_bending(void);
		}
	}
}
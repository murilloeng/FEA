#pragma once

namespace test
{
	namespace beam3D
	{
		void test(void);
		namespace elastic
		{
			void helix(void);
			void hockling_cable(void);
			void lee_frame(void);
			void dome_framed(void);
			void cantilever_arc(void);
			void deployable_ring(void);
			void cantilever_force(void);
			void frame_right_angle(void);
			void cantilever_bending(void);
			void cantilever_right_angle(void);
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
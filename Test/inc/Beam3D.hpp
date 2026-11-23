#pragma once

namespace test
{
	namespace beam3D
	{
		void test(void);
		namespace elastic
		{
			void helix(void);
			void roll_up(void);
			void lee_frame(void);
			void dome_framed(void);
			void cantilever_arc(void);
			void hockling_cable(void);
			void rigid_rotation(void);
			void deployable_ring(void);
			void lateral_buckling(void);
			void cantilever_force(void);
			void frame_right_angle(void);
			void path_independence(void);
			void cantilever_bending(void);
			void cantilever_right_angle(void);
		}
		namespace inelastic
		{
			void pinned_force(void);
			void pinned_moment(void);
			void cantilever_force(void);
			void cantilever_moment(void);
		}
	}
}
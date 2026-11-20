#pragma once

namespace test
{
	namespace beam2D
	{
		void test(void);
		namespace elastic
		{
			void lee_frame(void);
			void deep_arch(void);
			void euler_pinned(void);
			void square_frame(void);
			void euler_clamped(void);
			void pinched_ring(void);
			void diamond_frame(void);
			void honeycomb_grid(void);
			void euler_cantilever(void);
			void simply_supported(void);
			void cantilever_force(void);
			void cantilever_bending(void);
			void timoshenko_cantilever(void);
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
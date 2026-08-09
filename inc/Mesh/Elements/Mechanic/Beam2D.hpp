#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Beam.hpp"
#include "FEA/inc/Mesh/Elements/Mechanic/Fiber.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Beam2D : public Beam
			{
			private:
				//constructor
				Beam2D(void);

				//destructor
				~Beam2D(void);

			public:
				//data
				static uint32_t draw_mesh(void);
				static uint32_t draw_mesh(uint32_t);

			private:
				//data
				uint32_t stress_set(void) const override;
				uint32_t dof_set(uint32_t) const override;

				//tangents
				void inertia_CR(double*) const;
				void inertia_TL(double*) const;
				void inertia(double*) const override;

				void damping_CR(double*) const;
				void damping_TL(double*) const;
				void damping(double*) const override;

				void stiffness_CR(double*) const;
				void stiffness_TL(double*) const;
				void stiffness(double*) const override;

				//forces
				void internal_force_CR(double*) const;
				void internal_force_TL(double*) const;
				void internal_force(double*) const override;

				//analysis
				void update(void) override;
				void restore(void) override;

				//compute
				void compute_TL(void);
				void compute_CR(void);
				void compute(void) override;

				//compute CR
				void compute_CR_state(void);
				void compute_CR_elastic(void);
				void compute_CR_plastic(void);
				void compute_CR_plastic_section(double*, double);
				void compute_CR_plastic_kinematic(double*, double);

				//draw
				void draw_setup(draw::Data&) const override;
				void draw_update(draw::Data&) const override;

				//data
				double m_tr_old, m_tr_new;
				double m_dl[3], m_fl[3], m_Kl[9];

				//static
				static uint32_t m_draw_mesh;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}
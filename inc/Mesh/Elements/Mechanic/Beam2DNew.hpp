#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Beam.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Beam2DNew : public Beam
			{
			private:
				//constructor
				Beam2DNew(void);

				//destructor
				~Beam2DNew(void);

			public:
				//data
				double curvature(double);
				double curvature(void) const;

			private:
				//data
				uint32_t stress_set(void) const override;
				uint32_t dof_set(uint32_t) const override;

				//tangents
				void inertia(double*) const override;
				void damping(double*) const override;
				void stiffness(double*) const override;

				//forces
				void internal_force(double*) const override;

				//energies
				double kinetic_energy(void) const override;
				double internal_energy(void) const override;

				//analysis
				void compute(void) override;

				//draw
				void draw_setup(draw::Data&) const override;
				void draw_update(draw::Data&) const override;

				//data
				double m_curvature;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}
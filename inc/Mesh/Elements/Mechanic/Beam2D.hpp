#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Beam.hpp"

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

				//data
				uint32_t dof_set(uint32_t) const override;

				//tangents
				void inertia(double*) const override;
				void damping(double*) const override;
				void stiffness(double*) const override;

				//forces
				void internal_force(double*) const override;

				//analysis
				void compute(void) override;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}
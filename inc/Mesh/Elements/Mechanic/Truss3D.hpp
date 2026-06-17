#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Truss.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Truss3D : public Truss
			{
			private:
				//contructors
				Truss3D(void);

				//destructors
				~Truss3D(void) override;

			public:
				//types
				Type type(void) const override;
				uint32_t dof_set(uint32_t) const override;

			private:
				//tangents
				void inertia(double*) const override;
				void damping(double*) const override;
				void stiffness(double*) const override;

				//forces
				void internal_force(double*) const override;

				//energy
				double kinetic_energy(void) const override;
				double internal_energy(void) const override;

				//loads
				void load_line_force(double*, const boundary::loads::LineForce*) const;

				//friends
				friend class Element;
			};
		}
	}
}
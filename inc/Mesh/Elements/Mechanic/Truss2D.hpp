#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Mechanic/Truss.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Truss2D : public Truss
			{
			public:
				//constructor
				Truss2D(void);

				//destructor
				~Truss2D(void);

				//data
				uint32_t dof_set(uint32_t) const override;

				//tangents
				void inertia(double*) const override;
				void damping(double*) const override;
				void stiffness(double*) const override;

				//forces
				void internal_force(double*) const override;
			};
		}
	}
}
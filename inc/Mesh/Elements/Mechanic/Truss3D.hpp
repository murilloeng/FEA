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
			public:
				//constructor
				Truss3D(void);

				//destructor
				~Truss3D(void);

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
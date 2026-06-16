#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Element.hpp"

namespace fea
{
	namespace mesh
	{
		namespace elements
		{
			class Truss3D : public Element
			{
			public:
				//constructor
				Truss3D(void);
	
				//destructor
				~Truss3D(void);

				//type
				Type type(void) const override;

				//dof
				uint32_t dof(uint32_t) const override;

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
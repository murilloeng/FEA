#pragma once

//FEA
#include "FEA/inc/Mesh/Elements/Truss.hpp"

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

			private:
				//data
				double m_f, m_k;
			};
		}
	}
}
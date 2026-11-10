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
			private:
				//constructor
				Truss2D(void);

				//destructor
				~Truss2D(void);

				//data
				uint32_t dof_set(uint32_t) const override;

				//analysis
				void compute(analysis::Compute&) override;

				//friends
				friend class fea::mesh::Mesh;
			};
		}
	}
}
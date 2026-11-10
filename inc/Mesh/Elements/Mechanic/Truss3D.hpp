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
			protected:
				//constructor
				Truss3D(void);

				//destructor
				~Truss3D(void);

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